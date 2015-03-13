/*
 * Copyright (C) 2002-2012 Carsten Haitzler, Dan Sinclair, Mike Blumenkrantz,
 * Samsung Electronics. EFL.
 * That code is public domain and can be freely used or copied.
 * https://git.enlightenment.org/core/efl.git
 * https://enlightenment.org
 */

#include "groupedit_private.h"

static void
_fixed_point_divide_and_decompose_integer(int dividend, int divisor, int *int_part, int *frac_part)
{
   int quotient = ((long long)dividend << 16) / divisor;
   *frac_part = quotient & 0xffff;
   *int_part = quotient >> 16;
}

static void
_layout_dimension_change_min_max_cell_bound(int dim, int *new_dim, int min_d, int max_d, int cell_sz)
{
   if (dim > cell_sz)
     {
        if ((min_d != 0) && (cell_sz < min_d))
          *new_dim = min_d;
        else
          *new_dim = cell_sz;
     }
   else
     {
        if ((max_d != -1) && (cell_sz > max_d))
          *new_dim = max_d;
        else
          *new_dim = cell_sz;
     }
}

static void
_layout_set_offset_and_expand_dimension_space_max_bounded(int dim, int *new_dim, int space_sz, int max_dim, int *offset_current, int *offset_top, int *offset_bottom, double align, int pad_before, int pad_after)
{
   if (align >= 0.0)
     {
        *new_dim = dim;
        *offset_current = (space_sz - (dim + pad_before + pad_after)) * align
           + pad_before;
        *offset_top = pad_before;
        *offset_bottom = (space_sz - (dim + pad_before + pad_after))
           + pad_before;
     }
   else
     {
        if ((max_dim != -1) && (space_sz > max_dim))
          {
             *new_dim = max_dim;
             *offset_current = (space_sz - (max_dim + pad_before + pad_after)) * 0.5
                + pad_before;
             *offset_top = (space_sz - (max_dim + pad_before + pad_after)) * 0.5
                + pad_before;
             *offset_bottom = (space_sz - (max_dim + pad_before + pad_after)) * 0.5
                + pad_before;
          }
        else
          {
             *new_dim = space_sz;
             *offset_current = 0;
             *offset_top = 0;
             *offset_bottom = 0;
          }
     }
}

static void
_layout_set_offset_and_change_dimension_min_max_cell_bounded(int dim, int *new_dim, int min_dim, int max_dim, int cell_sz, int *offset_current, int *offset_top, int *offset_bottom, double align, int pad_before, int pad_after)
{
   if (align >= 0.0)
     {
        *new_dim = dim;
        *offset_current =
           (cell_sz - (dim + pad_before + pad_after)) * align + pad_before;
        *offset_top = pad_before;
        *offset_bottom =
           (cell_sz - (dim + pad_before + pad_after)) + pad_before;
     }
   else
     {
        *offset_current = pad_before;
        *offset_top = pad_before;
        *offset_bottom = pad_before;
        _layout_dimension_change_min_max_cell_bound
           (dim, new_dim, min_dim, max_dim, cell_sz - pad_before - pad_after);
     }
}

static void
_sizing_eval(Evas_Object *obj)
{
   Evas_Coord minw, minh, maxw, maxh;
   Evas_Coord w, h;

   evas_object_size_hint_min_get(obj, &minw, &minh);
   evas_object_size_hint_max_get(obj, &maxw, &maxh);
   evas_object_geometry_get(obj, NULL, NULL, &w, &h);

   if (w < minw) w = minw;
   if (h < minh) h = minh;
   if ((maxw >= 0) && (w > maxw)) w = maxw;
   if ((maxh >= 0) && (h > maxh)) h = maxh;

   evas_object_resize(obj, w, h);
}

static int
_evas_object_box_layout_horizontal_weight_apply(Evas_Object_Box_Data *priv, Evas_Object_Box_Option **objects, int n_objects, int remaining, double weight_total)
{
   int rem_diff = 0;
   int i;

   for (i = 0; i < n_objects; i++)
     {
        Evas_Object_Box_Option *opt = objects[i];
        Evas_Object *o = opt->obj;
        int h;

        evas_object_geometry_get(o, NULL, NULL, NULL, &h);

        if (remaining <= 0)
          {
             int min_w;

             evas_object_size_hint_min_get(o, &min_w, NULL);
             evas_object_resize(o, min_w, h);
          }
        else
          {
             double normal_weight, weight_x;
             int target_size;
             int max_w;

             evas_object_size_hint_weight_get(o, &weight_x, NULL);
             normal_weight = weight_x / weight_total;
             target_size = (int)((double)remaining * normal_weight);

             evas_object_size_hint_max_get(o, &max_w, NULL);
             if ((max_w != -1) && (target_size > max_w))
               {
                  evas_object_resize(o, max_w, h);
                  rem_diff += max_w;
                  objects[i] = objects[n_objects - 1];
                  weight_total -= weight_x;
                  n_objects--;
                  return _evas_object_box_layout_horizontal_weight_apply
                     (priv, objects, n_objects, remaining - rem_diff,
                      weight_total);
               }
             else
               {
                  evas_object_resize(o, target_size, h);
                  rem_diff += target_size;
               }
          }
     }

   return remaining - rem_diff;
}

void
_box_layout_horizontal(Evas_Box *o, Evas_Object_Box_Data *priv, void *data)
{
   int pad_inc = 0, sub_pixel = 0;
   int req_w, global_pad, remaining, top_h = 0;
   double weight_total = 0.0;
   int weight_use = 0;
   int x, y, w, h;
   int n_children;
   Evas_Object_Box_Option *opt;
   Evas_Object_Box_Option **objects;
   Eina_List *l;
   Groupedit_Item *ge_item = NULL;

   Eina_List *items = (Eina_List *)data;

   n_children = eina_list_count(priv->children);
   if (!n_children)
     {
        evas_object_size_hint_min_set(o, 0, 0);
        return;
     }

   objects = (Evas_Object_Box_Option **)alloca(sizeof(Evas_Object_Box_Option *) * n_children);
   if (!objects)
     {
        evas_object_size_hint_min_set(o, 0, 0);
        return;
     }

   evas_object_geometry_get(o, &x, &y, &w, &h);
   global_pad = priv->pad.h;
   req_w = global_pad * (n_children - 1);

   EINA_LIST_FOREACH(priv->children, l, opt)
     {
        int padding_l, padding_r;
        double weight_x;

        _sizing_eval(opt->obj);
        evas_object_size_hint_weight_get(opt->obj, &weight_x, NULL);
        evas_object_size_hint_padding_get
           (opt->obj, &padding_l, &padding_r, NULL, NULL);
        req_w += padding_l + padding_r;

        if (!weight_x)
          {
             int child_w;

             evas_object_geometry_get(opt->obj, NULL, NULL, &child_w, NULL);
             req_w += child_w;
          }
        else
          {
             objects[weight_use] = opt;
             weight_use++;
             weight_total += weight_x;
          }
     }

   remaining = w - req_w;

   if (weight_use)
     remaining = _evas_object_box_layout_horizontal_weight_apply
       (priv, objects, weight_use, remaining, weight_total);

   if (priv->align.h >= 0.0)
     x += remaining * priv->align.h;
   else if (n_children == 1)
     x += remaining / 2;
   else
     { /* justified */
        _fixed_point_divide_and_decompose_integer
            (remaining, n_children - 1, &global_pad, &pad_inc);
        global_pad += priv->pad.h;
     }

   EINA_LIST_FOREACH(priv->children, l, opt)
     {
        ge_item = eina_list_data_get(items);

        int child_w, child_h, max_h, new_h, off_x, off_y, off_y_t, off_y_b;
        int padding_l, padding_r, padding_t, padding_b;
        double align_y;

        evas_object_size_hint_align_get(opt->obj, NULL, &align_y);
        evas_object_size_hint_padding_get
           (opt->obj, &padding_l, &padding_r, &padding_t, &padding_b);
        evas_object_size_hint_max_get(opt->obj, NULL, &max_h);

        evas_object_geometry_get(opt->obj, NULL, NULL, &child_w, &child_h);

        off_x = padding_l;
        new_h = child_h;
        if (new_h > top_h) top_h = new_h;

        _layout_set_offset_and_expand_dimension_space_max_bounded
           (child_h, &new_h, h, max_h, &off_y, &off_y_t, &off_y_b, align_y, padding_t, padding_b);

        if (new_h != child_h)
          evas_object_resize(opt->obj, child_w, new_h);
        evas_object_move(opt->obj, x + off_x, y + off_y);

        evas_object_resize(ge_item->highlight, child_w, new_h);
        evas_object_move(ge_item->highlight, x + off_x, y + off_y);

        evas_object_resize(ge_item->border, child_w, new_h + abs(off_y_t - off_y_b));

        if (align_y >= 0)
          off_y = (off_y_t > off_y_b) ? off_y_b : off_y_t;

        evas_object_move(ge_item->border, x + off_x, y + off_y);

        x += child_w + padding_l + padding_r + global_pad;
        sub_pixel += pad_inc;
        if (sub_pixel >= 1 << 16)
          {
             x++;
             sub_pixel -= 1 << 16;
          }
        items = eina_list_next(items);
     }

   evas_object_size_hint_min_set(o, req_w, top_h);
}

static int
_evas_object_box_layout_vertical_weight_apply(Evas_Object_Box_Data *priv, Evas_Object_Box_Option **objects, int n_objects, int remaining, double weight_total)
{
   int rem_diff = 0;
   int i;

   for (i = 0; i < n_objects; i++)
     {
        Evas_Object_Box_Option *opt = objects[i];
        Evas_Object *o = opt->obj;
        int w;

        evas_object_geometry_get(o, NULL, NULL, &w, NULL);

        if (remaining <= 0)
          {
             int min_h;

             evas_object_size_hint_min_get(o, NULL, &min_h);
             evas_object_resize(o, w, min_h);
          }
        else
          {
             double normal_weight, weight_y;
             int target_size;
             int max_h;

             evas_object_size_hint_weight_get(o, NULL, &weight_y);
             normal_weight = weight_y / weight_total;
             target_size = (int)((double)remaining * normal_weight);

             evas_object_size_hint_max_get(o, NULL, &max_h);
             if ((max_h != -1) && (target_size > max_h))
               {
                  evas_object_resize(o, w, max_h);
                  rem_diff += max_h;
                  objects[i] = objects[n_objects - 1];
                  weight_total -= weight_y;
                  n_objects--;
                  return _evas_object_box_layout_vertical_weight_apply
                     (priv, objects, n_objects, remaining - rem_diff,
                      weight_total);
               }
             else
               {
                  evas_object_resize(o, w, target_size);
                  rem_diff += target_size;
               }
          }
     }

   return remaining - rem_diff;
}

void
_box_layout_vertical(Evas_Box *o, Evas_Object_Box_Data *priv, void *data)
{
   int pad_inc = 0, sub_pixel = 0;
   int req_h, global_pad, remaining, top_w = 0;
   double weight_total = 0.0;
   int weight_use = 0;
   int x, y, w, h;
   int n_children;
   Evas_Object_Box_Option *opt;
   Evas_Object_Box_Option **objects;
   Eina_List *l;
   Groupedit_Item *ge_item = NULL;

   Eina_List *items = (Eina_List *)data;

   n_children = eina_list_count(priv->children);
   if (!n_children)
     {
        evas_object_size_hint_min_set(o, 0, 0);
        return;
     }

   objects = (Evas_Object_Box_Option **)alloca(sizeof(Evas_Object_Box_Option *) * n_children);
   if (!objects)
     {
        evas_object_size_hint_min_set(o, 0, 0);
        return;
     }

   evas_object_geometry_get(o, &x, &y, &w, &h);
   global_pad = priv->pad.v;
   req_h = global_pad * (n_children - 1);

   EINA_LIST_FOREACH(priv->children, l, opt)
     {
        int padding_t, padding_b;
        double weight_y;

        _sizing_eval(opt->obj);
        evas_object_size_hint_weight_get(opt->obj, NULL, &weight_y);
        evas_object_size_hint_padding_get
           (opt->obj, NULL, NULL, &padding_t, &padding_b);
        req_h += padding_t + padding_b;

        if (!weight_y)
          {
             int child_h;

             evas_object_geometry_get(opt->obj, NULL, NULL, NULL, &child_h);
             req_h += child_h;
          }
        else
          {
             objects[weight_use] = opt;
             weight_use++;
             weight_total += weight_y;
          }
     }

   remaining = h - req_h;

   if (weight_use)
     remaining = _evas_object_box_layout_vertical_weight_apply
       (priv, objects, weight_use, remaining, weight_total);

   if (priv->align.v >= 0.0)
     y += remaining * priv->align.v;
   else if (n_children == 1)
     y += remaining / 2;
   else
     { /* justified */
        _fixed_point_divide_and_decompose_integer
           (remaining, n_children - 1, &global_pad, &pad_inc);
        global_pad += priv->pad.v;
     }

   EINA_LIST_FOREACH(priv->children, l, opt)
     {
        ge_item = eina_list_data_get(items);

        int child_w, child_h, max_w, new_w, off_x, off_x_t, off_x_b, off_y;
        int padding_l, padding_r, padding_t, padding_b;
        double align_x;

        evas_object_size_hint_align_get(opt->obj, &align_x, NULL);
        evas_object_size_hint_padding_get
           (opt->obj, &padding_l, &padding_r, &padding_t, &padding_b);
        evas_object_size_hint_max_get(opt->obj, &max_w, NULL);

        evas_object_geometry_get(opt->obj, NULL, NULL, &child_w, &child_h);

        off_y = padding_t;
        new_w = child_w;

        _layout_set_offset_and_expand_dimension_space_max_bounded
           (child_w, &new_w, w, max_w, &off_x, &off_x_t, &off_x_b, align_x, padding_l, padding_r);

        if (new_w > top_w) top_w = new_w;

        if (new_w != child_w)
          evas_object_resize(opt->obj, new_w, child_h);
        evas_object_move(opt->obj, x + off_x, y + off_y);

        evas_object_resize(ge_item->highlight, new_w, child_h);
        evas_object_move(ge_item->highlight, x + off_x, y + off_y);

        evas_object_resize(ge_item->border, new_w + abs(off_x_t - off_x_b), child_h);

        if (align_x >= 0)
          off_x = (off_x_t > off_x_b) ? off_x_b : off_x_t;

        evas_object_move(ge_item->border, x + off_x, y + off_y);

        y += child_h + padding_t + padding_b + global_pad;
        sub_pixel += pad_inc;
        if (sub_pixel >= 1 << 16)
          {
             y++;
             sub_pixel -= 1 << 16;
          }
        items = eina_list_next(items);
     }

   evas_object_size_hint_min_set(o, top_w, req_h);
}

void
_box_layout_homogeneous_horizontal(Evas_Box *o, Evas_Object_Box_Data *priv, void *data)
{
   int cell_sz, share, inc;
   int sub_pixel = 0;
   int x, y, w, h;
   int n_children;
   Evas_Object_Box_Option *opt;
   Eina_List *l;
   Groupedit_Item *ge_item = NULL;

   Eina_List *items = (Eina_List *)data;

   n_children = eina_list_count(priv->children);
   if (!n_children)
     {
        evas_object_size_hint_min_set(o, 0, 0);
        return;
     }

   evas_object_geometry_get(o, &x, &y, &w, &h);

   share = w - priv->pad.h * (n_children - 1);
   _fixed_point_divide_and_decompose_integer
     (share, n_children, &cell_sz, &inc);

   EINA_LIST_FOREACH(priv->children, l, opt)
     {
        ge_item = eina_list_data_get(items);

        int child_w, child_h, max_h, min_w, max_w, new_w, new_h;
        int off_x, off_x_t, off_x_b, off_y_b, off_y_t, off_y;
        int padding_l, padding_r, padding_t, padding_b;
        double align_x, align_y;

        evas_object_size_hint_align_get(opt->obj, &align_x, &align_y);
        evas_object_size_hint_padding_get
           (opt->obj, &padding_l, &padding_r, &padding_t, &padding_b);
        evas_object_size_hint_max_get(opt->obj, &max_w, &max_h);
        evas_object_size_hint_min_get(opt->obj, &min_w, NULL);

        _sizing_eval(opt->obj);
        evas_object_geometry_get(opt->obj, NULL, NULL, &child_w, &child_h);

        new_w = child_w;
        new_h = child_h;

        _layout_set_offset_and_expand_dimension_space_max_bounded
           (child_h, &new_h, h, max_h, &off_y, &off_y_t, &off_y_b, align_y, padding_t, padding_b);

        _layout_set_offset_and_change_dimension_min_max_cell_bounded
           (child_w, &new_w, min_w, max_w, cell_sz, &off_x, &off_x_t, &off_x_b, align_x,
            padding_l, padding_r);

        if ((new_w != child_w) || (new_h != child_h))
          evas_object_resize(opt->obj, new_w, new_h);
        evas_object_move(opt->obj, x + off_x, y + off_y);

        evas_object_resize(ge_item->highlight, new_w, new_h);
        evas_object_move(ge_item->highlight, x + off_x, y + off_y);

        evas_object_resize(ge_item->border,
                           new_w + abs(off_x_t - off_x_b),
                           new_h + abs(off_y_t - off_y_b));

        if (align_x >= 0)
          off_x = (off_x_t > off_x_b) ? off_x_b : off_x_t;
        if (align_y >= 0)
          off_y = (off_y_t > off_y_b) ? off_y_b : off_y_t;

        evas_object_move(ge_item->border, x + off_x, y + off_y);

        x += cell_sz + priv->pad.h;
        sub_pixel += inc;
        if (sub_pixel >= 1 << 16)
          {
             x++;
             sub_pixel -= 1 << 16;
          }
        items = eina_list_next(items);
     }

   evas_object_size_hint_min_set(o, w, h);
}

void
_box_layout_homogeneous_vertical(Evas_Box *o, Evas_Object_Box_Data *priv, void *data)
{
   int cell_sz, share, inc;
   int sub_pixel = 0;
   int x, y, w, h;
   int n_children;
   Evas_Object_Box_Option *opt;
   Eina_List *l;
   Groupedit_Item *ge_item = NULL;

   Eina_List *items = (Eina_List *)data;

   n_children = eina_list_count(priv->children);
   if (!n_children)
     {
        evas_object_size_hint_min_set(o, 0, 0);
        return;
     }

   evas_object_geometry_get(o, &x, &y, &w, &h);

   share = h - priv->pad.v * (n_children - 1);
   _fixed_point_divide_and_decompose_integer
     (share, n_children, &cell_sz, &inc);

   EINA_LIST_FOREACH(priv->children, l, opt)
     {
        ge_item = eina_list_data_get(items);

        int child_w, child_h, max_w, min_h, max_h, new_w, new_h;
        int off_x, off_x_t, off_x_b, off_y, off_y_t, off_y_b;
        int padding_l, padding_r, padding_t, padding_b;
        double align_x, align_y;

        evas_object_size_hint_align_get(opt->obj, &align_x, &align_y);
        evas_object_size_hint_padding_get
           (opt->obj, &padding_l, &padding_r, &padding_t, &padding_b);
        evas_object_size_hint_max_get(opt->obj, &max_w, &max_h);
        evas_object_size_hint_min_get(opt->obj, NULL, &min_h);

        _sizing_eval(opt->obj);
        evas_object_geometry_get(opt->obj, NULL, NULL, &child_w, &child_h);
        new_w = child_w;
        new_h = child_h;

        _layout_set_offset_and_expand_dimension_space_max_bounded
           (child_w, &new_w, w, max_w, &off_x, &off_x_t, &off_x_b, align_x, padding_l, padding_r);

        _layout_set_offset_and_change_dimension_min_max_cell_bounded
           (child_h, &new_h, min_h, max_h, cell_sz, &off_y, &off_y_t, &off_y_b, align_y,
            padding_t, padding_b);

        if ((new_w != child_w) || (new_h != child_h))
          evas_object_resize(opt->obj, new_w, new_h);
        evas_object_move(opt->obj, x + off_x, y + off_y);

        evas_object_resize(ge_item->highlight, new_w, new_h);
        evas_object_move(ge_item->highlight, x + off_x, y + off_y);

        evas_object_resize(ge_item->border,
                           new_w + abs(off_x_t - off_x_b),
                           new_h + abs(off_y_t - off_y_b));

        if (align_x >= 0)
          off_x = (off_x_t > off_x_b) ? off_x_b : off_x_t;
        if (align_y >= 0)
          off_y = (off_y_t > off_y_b) ? off_y_b : off_y_t;

        evas_object_move(ge_item->border, x + off_x, y + off_y);

        y += cell_sz + priv->pad.v;
        sub_pixel += inc;
        if (sub_pixel >= 1 << 16)
          {
             y++;
             sub_pixel -= 1 << 16;
          }
        items = eina_list_next(items);
     }

   evas_object_size_hint_min_set(o, w, h);
}

void
_box_layout_homogeneous_max_size_horizontal(Evas_Box *o, Evas_Object_Box_Data *priv, void *data)
{
   int remaining, global_pad, pad_inc = 0, sub_pixel = 0;
   int cell_sz = 0;
   int x, y, w, h;
   int top_h = 0;
   int n_children;
   Evas_Object_Box_Option *opt;
   Eina_List *l;
   Groupedit_Item *ge_item = NULL;

   Eina_List *items = (Eina_List *)data;

   n_children = eina_list_count(priv->children);
   if (!n_children)
     {
        evas_object_size_hint_min_set(o, 0, 0);
        return;
     }

   evas_object_geometry_get(o, &x, &y, &w, &h);

   EINA_LIST_FOREACH(priv->children, l, opt)
     {
        int child_w, padding_l, padding_r;

        _sizing_eval(opt->obj);
        evas_object_size_hint_padding_get
           (opt->obj, &padding_l, &padding_r, NULL, NULL);
        evas_object_geometry_get(opt->obj, NULL, NULL, &child_w, NULL);
        if (child_w + padding_l + padding_r > cell_sz)
          cell_sz = child_w + padding_l + padding_r;
     }

   global_pad = priv->pad.h;
   remaining = w - n_children * cell_sz - global_pad * (n_children - 1);

   if (priv->align.h >= 0.0)
     x += remaining * priv->align.h;
   else if (n_children == 1)
     x += remaining / 2;
   else
     { /* justified */
        _fixed_point_divide_and_decompose_integer
           (remaining, n_children - 1, &global_pad, &pad_inc);
        global_pad += priv->pad.h;
     }

   EINA_LIST_FOREACH(priv->children, l, opt)
     {
        ge_item = eina_list_data_get(items);

        int child_w, child_h, min_w, max_w, max_h, new_w, new_h;
        int off_x, off_x_t, off_x_b, off_y, off_y_t, off_y_b;
        int padding_l, padding_r, padding_t, padding_b;
        double align_x, align_y;

        evas_object_size_hint_align_get(opt->obj, &align_x, &align_y);
        evas_object_size_hint_padding_get
           (opt->obj, &padding_l, &padding_r, &padding_t, &padding_b);
        evas_object_size_hint_max_get(opt->obj, &max_w, &max_h);
        evas_object_size_hint_min_get(opt->obj, &min_w, NULL);

        evas_object_geometry_get(opt->obj, NULL, NULL, &child_w, &child_h);

        new_w = child_w;
        new_h = child_h;
        if (new_h > top_h) top_h = new_h;

        _layout_set_offset_and_expand_dimension_space_max_bounded
           (child_h, &new_h, h, max_h, &off_y, &off_y_t, &off_y_b, align_y, padding_t, padding_b);

        _layout_set_offset_and_change_dimension_min_max_cell_bounded
           (child_w, &new_w, min_w, max_w, cell_sz, &off_x, &off_x_t, &off_x_b, align_x,
            padding_l, padding_r);

        if ((new_w != child_w) || (new_h != child_h))
          evas_object_resize(opt->obj, new_w, new_h);
        evas_object_move(opt->obj, x + off_x, y + off_y);

        evas_object_resize(ge_item->highlight, new_w, new_h);
        evas_object_move(ge_item->highlight, x + off_x, y + off_y);

        evas_object_resize(ge_item->border,
                           new_w + abs(off_x_t - off_x_b),
                           new_h + abs(off_y_t - off_y_b));

        if (align_x >= 0)
          off_x = (off_x_t > off_x_b) ? off_x_b : off_x_t;
        if (align_y >= 0)
          off_y = (off_y_t > off_y_b) ? off_y_b : off_y_t;

        evas_object_move(ge_item->border, x + off_x, y + off_y);

        x += cell_sz + global_pad;
        sub_pixel += pad_inc;
        if (sub_pixel >= 1 << 16)
          {
             x++;
             sub_pixel -= 1 << 16;
          }
        items = eina_list_next(items);
     }

   evas_object_size_hint_min_set(o, x, top_h);
}

void
_box_layout_homogeneous_max_size_vertical(Evas_Box *o, Evas_Object_Box_Data *priv, void *data)
{
   int remaining, global_pad, pad_inc = 0, sub_pixel = 0;
   int cell_sz = 0;
   int x, y, w, h;
   int top_w = 0;
   int n_children;
   Evas_Object_Box_Option *opt;
   Eina_List *l;
   Groupedit_Item *ge_item = NULL;

   Eina_List *items = (Eina_List *)data;

   n_children = eina_list_count(priv->children);
   if (!n_children)
     {
        evas_object_size_hint_min_set(o, 0, 0);
        return;
     }

   evas_object_geometry_get(o, &x, &y, &w, &h);

   EINA_LIST_FOREACH(priv->children, l, opt)
     {
        int child_h, padding_t, padding_b;

        _sizing_eval(opt->obj);
        evas_object_size_hint_padding_get
           (opt->obj, NULL, NULL, &padding_t, &padding_b);
        evas_object_geometry_get(opt->obj, NULL, NULL, NULL, &child_h);
        if (child_h + padding_t + padding_b > cell_sz)
          cell_sz = child_h + padding_t + padding_b;
     }

   global_pad = priv->pad.v;
   remaining = h - n_children * cell_sz - global_pad * (n_children - 1);

   if (priv->align.v >= 0.0)
     y += remaining * priv->align.v;
   else if (n_children == 1)
     y += remaining / 2;
   else
     { /* justified */
        _fixed_point_divide_and_decompose_integer
           (remaining, n_children - 1, &global_pad, &pad_inc);
        global_pad += priv->pad.v;
     }

   EINA_LIST_FOREACH(priv->children, l, opt)
     {
        ge_item = eina_list_data_get(items);

        int child_w, child_h, max_h, min_h, max_w, new_w, new_h;
        int off_x, off_x_t, off_x_b, off_y, off_y_t, off_y_b;
        int padding_l, padding_r, padding_t, padding_b;
        double align_x, align_y;

        evas_object_size_hint_align_get(opt->obj, &align_x, &align_y);
        evas_object_size_hint_padding_get
           (opt->obj, &padding_l, &padding_r, &padding_t, &padding_b);
        evas_object_size_hint_max_get(opt->obj, &max_w, &max_h);
        evas_object_size_hint_min_get(opt->obj, NULL, &min_h);

        evas_object_geometry_get(opt->obj, NULL, NULL, &child_w, &child_h);

        new_w = child_w;
        new_h = child_h;
        if (new_w > top_w) top_w = new_w;

        _layout_set_offset_and_expand_dimension_space_max_bounded
           (child_w, &new_w, w, max_w, &off_x, &off_x_t, &off_x_b, align_x, padding_l, padding_r);

        _layout_set_offset_and_change_dimension_min_max_cell_bounded
           (child_h, &new_h, min_h, max_h, cell_sz, &off_y, &off_y_t, &off_y_b, align_y,
            padding_t, padding_b);

        if ((new_w != child_w) || (new_h != child_h))
          evas_object_resize(opt->obj, new_w, new_h);
        evas_object_move(opt->obj, x + off_x, y + off_y);

        evas_object_resize(ge_item->highlight, new_w, new_h);
        evas_object_move(ge_item->highlight, x + off_x, y + off_y);

        evas_object_resize(ge_item->border,
                           new_w + abs(off_x_t - off_x_b),
                           new_h + abs(off_y_t - off_y_b));

        if (align_x >= 0)
          off_x = (off_x_t > off_x_b) ? off_x_b : off_x_t;
        if (align_y >= 0)
          off_y = (off_y_t > off_y_b) ? off_y_b : off_y_t;

        evas_object_move(ge_item->border, x + off_x, y + off_y);

        y += cell_sz + global_pad;
        sub_pixel += pad_inc;
        if (sub_pixel >= 1 << 16)
          {
             y++;
             sub_pixel -= 1 << 16;
          }
        items = eina_list_next(items);
     }

   evas_object_size_hint_min_set(o, top_w, y);
}

void
_evas_object_box_layout_flow_horizontal_row_info_collect(Evas_Object_Box_Data *priv, int box_w, int *row_count, int *row_max_h, int *row_break, int *row_width, int *off_y_ret, int *max_w_ret, int *max_h_ret)
{
   int i, remain_w = box_w, start_i = 0;
   int off_y = 0, max_w = 0, max_h = 0, n_rows = 0;
   Eina_List *l;

   for (i = 0, l = priv->children; l; i++, l = l->next)
     {
        Evas_Object_Box_Option *opt = l->data;
        int padding_l, padding_r, padding_t, padding_b;
        int child_w, child_h, off_x = 0;

        evas_object_size_hint_padding_get
           (opt->obj, &padding_l, &padding_r, &padding_t, &padding_b);

        _sizing_eval(opt->obj);
        evas_object_geometry_get(opt->obj, NULL, NULL, &child_w, &child_h);

        child_w += padding_l + padding_r + priv->pad.h;
        child_h += padding_t + padding_b;
        if (child_w > max_w)
          max_w = child_w;

        remain_w -= child_w;
        if (remain_w + priv->pad.h >= 0)
          { /* continue "line" */
             if (child_h > max_h)
               max_h = child_h;

             off_x += child_w;
             row_width[n_rows] += child_w;
          }
        else
          { /* break line */
             if (i == start_i)
               { /* obj goes to actual line */
                  max_h = child_h;
                  row_width[n_rows] = child_w;
               }
             else
               { /* obj goes to next line */
                  row_max_h[n_rows] = max_h;
                  row_break[n_rows] = i - 1;
                  n_rows++;

                  off_x = child_w;
                  off_y += max_h;
                  max_h = child_h;

                  row_width[n_rows] = child_w;
                  start_i = i;

                  remain_w = box_w - off_x;
               }
          }
     }

   row_break[n_rows] = i - 1;
   row_max_h[n_rows] = max_h;

   *row_count = n_rows;
   *off_y_ret = off_y;
   *max_w_ret = max_w;
   *max_h_ret = max_h;
}

void
_box_layout_flow_horizontal(Evas_Box *o, Evas_Object_Box_Data *priv, void *data __UNUSED__)
{
   int n_children;
   int r, row_count = 0;
   int min_w = 0, min_h = 0;
   int max_h, inc_y;
   int remain_y, i;
   int x, y, w, h;
   Eina_List *l;
   int *row_max_h;
   int *row_break;
   int *row_width;
   int offset_y;

   n_children = eina_list_count(priv->children);
   if (!n_children)
     {
        evas_object_size_hint_min_set(o, 0, 0);
        return;
     }

   /* *per row* arrays */
   row_max_h = (int *)alloca(sizeof(int) * n_children);
   row_break = (int *)alloca(sizeof(int) * n_children);
   row_width = (int *)alloca(sizeof(int) * n_children);

   memset(row_width, 0, sizeof(int) * n_children);

   evas_object_geometry_get(o, &x, &y, &w, &h);

   _evas_object_box_layout_flow_horizontal_row_info_collect
     (priv, w, &row_count, row_max_h, row_break, row_width, &offset_y, &min_w, &max_h);

   inc_y = 0;
   remain_y = h - (offset_y + max_h);

   if (remain_y > 0)
     {
        if (priv->align.v >= 0.0)
          inc_y = priv->align.v * remain_y;
        else if (row_count == 0)
          y += remain_y / 2;
        else /* y-justified */
          inc_y = remain_y / row_count;
     }

   inc_y += priv->pad.v;

   for (i = 0, r = 0, l = priv->children; r <= row_count; r++)
     {
        int row_justify = 0, just_inc = 0, sub_pixel = 0;
        int row_size, remain_x;

        row_size = row_break[r] - i;
        remain_x = (w - row_width[r]);

        if (priv->align.h < 0.0)
          {
             if (row_size == 0)
               x += remain_x / 2;
             else
               _fixed_point_divide_and_decompose_integer
                  (remain_x, row_size, &row_justify, &just_inc);
          }

        row_justify += priv->pad.h;

        for (; i <= row_break[r]; i++, l = l->next)
          {
             Evas_Object_Box_Option *opt = l->data;
             int off_x, off_y, y_remain;
             int padding_l, padding_r;
             int child_w, child_h;
             double align_y;

             evas_object_size_hint_align_get(opt->obj, NULL, &align_y);
             evas_object_size_hint_padding_get
                (opt->obj, &padding_l, &padding_r, NULL, NULL);

             evas_object_geometry_get
                (opt->obj, NULL, NULL, &child_w, &child_h);

             y_remain = row_max_h[r] - child_h;

             off_x = padding_l;
             if (priv->align.h >= 0.0)
               off_x += remain_x * priv->align.h;
             off_y = y_remain * align_y;

             evas_object_move(opt->obj, x + off_x, y + off_y);

             x += child_w + padding_l + padding_r + row_justify;

             sub_pixel += just_inc;
             if (sub_pixel >= 1 << 16)
               {
                  x++;
                  sub_pixel -= 1 << 16;
               }
          }

        evas_object_geometry_get(o, &x, NULL, NULL, NULL);
        min_h += row_max_h[r];
        y += row_max_h[r] + inc_y;
     }

   evas_object_size_hint_min_set(o, min_w, min_h);
}

static void
_evas_object_box_layout_flow_vertical_col_info_collect(Evas_Object_Box_Data *priv, int box_h, int *col_count, int *col_max_w, int *col_break, int *col_height, int *off_x_ret, int *max_w_ret, int *max_h_ret)
{
   int i, remain_h = box_h, start_i = 0;
   int off_x = 0, max_w = 0, max_h = 0, n_cols = 0;
   Eina_List *l;

   for (i = 0, l = priv->children; l; i++, l = l->next)
     {
        Evas_Object_Box_Option *opt = l->data;
        int padding_l, padding_r, padding_t, padding_b;
        int child_w, child_h, off_y = 0;

        evas_object_size_hint_padding_get
           (opt->obj, &padding_l, &padding_r, &padding_t, &padding_b);

        _sizing_eval(opt->obj);
        evas_object_geometry_get(opt->obj, NULL, NULL, &child_w, &child_h);

        child_w += padding_l + padding_r;
        child_h += padding_t + padding_b + priv->pad.v;
        if (child_h > max_h)
          max_h = child_h;

        remain_h -= child_h;
        if (remain_h + priv->pad.v >= 0)
          { /* continue "col" */
             if (child_w > max_w)
               max_w = child_w;

             off_y += child_h;
             col_height[n_cols] += child_h;
          }
        else
          {
             /* break col */
             if (i == start_i)
               { /* obj goes to actual col */
                  max_w = child_w;
                  col_height[n_cols] = child_h;
               }
             else
               { /* obj goes to next col */
                  col_max_w[n_cols] = max_w;
                  col_break[n_cols] = i - 1;
                  n_cols++;

                  off_x += max_w;
                  off_y = child_h;
                  max_w = child_w;

                  col_height[n_cols] = child_h;
                  start_i = i;

                  remain_h = box_h - off_y;
               }
          }
     }

   col_break[n_cols] = i - 1;
   col_max_w[n_cols] = max_w;

   *col_count = n_cols;
   *off_x_ret = off_x;
   *max_w_ret = max_w;
   *max_h_ret = max_h;
}

void
_box_layout_flow_vertical(Evas_Box *o, Evas_Object_Box_Data *priv, void *data __UNUSED__)
{
   int n_children;
   int c, col_count;
   int min_w = 0, min_h = 0;
   int max_w, inc_x;
   int remain_x, i;
   int x, y, w, h;
   Eina_List *l;
   int *col_max_w;
   int *col_break;
   int *col_height;
   int offset_x;

   n_children = eina_list_count(priv->children);
   if (!n_children)
     {
        evas_object_size_hint_min_set(o, 0, 0);
        return;
     }

   /* *per col* arrays */
   col_max_w = (int *)alloca(sizeof(int) * n_children);
   col_break = (int *)alloca(sizeof(int) * n_children);
   col_height = (int *)alloca(sizeof(int) * n_children);

   memset(col_height, 0, sizeof(int) * n_children);

   evas_object_geometry_get(o, &x, &y, &w, &h);

   _evas_object_box_layout_flow_vertical_col_info_collect
     (priv, h, &col_count, col_max_w, col_break, col_height, &offset_x, &max_w, &min_h);

   inc_x = 0;
   remain_x = w - (offset_x + max_w);

   if (remain_x > 0)
     {
        if (priv->align.h >= 0)
          inc_x = priv->align.h * remain_x;
        else if (col_count == 0)
          x += remain_x / 2;
        else /* x-justified */
          inc_x = remain_x / col_count;
     }

   inc_x += priv->pad.h;

   for (i = 0, c = 0, l = priv->children; c <= col_count; c++)
     {
        int col_justify = 0, just_inc = 0, sub_pixel = 0;
        int col_size, remain_y;

        col_size = col_break[c] - i;
        remain_y = (h - col_height[c]);

        if (priv->align.v < 0.0)
          {
             if (col_size == 0)
               y += remain_y / 2;
             else
               _fixed_point_divide_and_decompose_integer
                  (remain_y, col_size, &col_justify, &just_inc);
          }

        col_justify += priv->pad.v;

        for (; i <= col_break[c]; i++, l = l->next)
          {
             Evas_Object_Box_Option *opt = l->data;
             int off_x, off_y, x_remain;
             int padding_t, padding_b;
             int child_w, child_h;
             double align_x;

             evas_object_size_hint_align_get(opt->obj, &align_x, NULL);
             evas_object_size_hint_padding_get
                (opt->obj, NULL, NULL, &padding_t, &padding_b);

             evas_object_geometry_get
                (opt->obj, NULL, NULL, &child_w, &child_h);

             x_remain = col_max_w[c] - child_w;

             off_x = x_remain * align_x;
             off_y = padding_t;
             if (priv->align.v >= 0.0)
               off_y += remain_y * priv->align.v;

             evas_object_move(opt->obj, x + off_x, y + off_y);

             y += child_h + padding_t + padding_b + col_justify;

             sub_pixel += just_inc;
             if (sub_pixel >= 1 << 16)
               {
                  y++;
                  sub_pixel -= 1 << 16;
               }
          }

        evas_object_geometry_get(o, NULL, &y, NULL, NULL);
        min_w += col_max_w[c];
        x += col_max_w[c] + inc_x;
     }

   evas_object_size_hint_min_set(o, min_w, min_h);
}

void
_box_layout_stack(Evas_Box *o, Evas_Object_Box_Data *priv, void *data)
{
   Eina_List *l;
   Evas_Coord ox, oy, ow, oh;
   Evas_Coord top_w = 0, top_h = 0;
   Evas_Object_Box_Option *opt;
   Evas_Object *old_child = NULL;
   Groupedit_Item *ge_item = NULL;

   Eina_List *items = (Eina_List *)data;

   evas_object_geometry_get(o, &ox, &oy, &ow, &oh);

   EINA_LIST_FOREACH(priv->children, l, opt)
     {
        ge_item = eina_list_data_get(items);

        Evas_Object *child = opt->obj;
        Evas_Coord max_w, max_h, min_w, min_h, pad_l, pad_r, pad_t, pad_b;
        Evas_Coord child_w, child_h, new_w, new_h;
        Evas_Coord off_x, off_x_t, off_x_b, off_y, off_y_t, off_y_b;
        double align_x, align_y;

        evas_object_size_hint_align_get(child, &align_x, &align_y);
        evas_object_size_hint_padding_get
           (child, &pad_l, &pad_r, &pad_t, &pad_b);
        evas_object_size_hint_max_get(child, &max_w, &max_h);
        evas_object_size_hint_min_get(child, &min_w, &min_h);

        _sizing_eval(opt->obj);
        evas_object_geometry_get(child, NULL, NULL, &child_w, &child_h);
        new_w = child_w;
        new_h = child_h;

        _layout_set_offset_and_change_dimension_min_max_cell_bounded
           (child_w, &new_w, min_w, max_w, ow, &off_x, &off_x_t, &off_x_b, align_x, pad_l, pad_r);
        _layout_set_offset_and_change_dimension_min_max_cell_bounded
           (child_h, &new_h, min_h, max_h, oh, &off_y, &off_y_t, &off_y_b, align_y, pad_t, pad_b);

        if (new_w > top_w) top_w = new_w;
        if (new_h > top_h) top_h = new_h;

        if ((new_w != child_w) || (new_h != child_h))
          evas_object_resize(child, new_w, new_h);
        evas_object_move(child, ox + off_x, oy + off_y);

        evas_object_resize(ge_item->highlight, new_w, new_h);
        evas_object_move(ge_item->highlight, ox + off_x, oy + off_y);

        evas_object_resize(ge_item->border,
                           new_w + abs(off_x_t - off_x_b),
                           new_h + abs(off_y_t - off_y_b));

        if (align_x >= 0)
          off_x = (off_x_t > off_x_b) ? off_x_b : off_x_t;
        if (align_y >= 0)
          off_y = (off_y_t > off_y_b) ? off_y_b : off_y_t;

        evas_object_move(ge_item->border, ox + off_x, oy + off_y);

        if (old_child)
          evas_object_stack_above(child, old_child);
        old_child = child;

        items = eina_list_next(items);
     }

   evas_object_size_hint_min_set(o, top_w, top_h);
}

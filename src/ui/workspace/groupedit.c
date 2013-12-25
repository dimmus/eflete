#include "groupedit_private.h"

#define MY_CLASS_NAME "Ws_Groupedit"

#define H_WIGTH (sd->handler_TL.w + sd->handler_BR.w)
#define H_HEIGHT (sd->handler_TL.h + sd->handler_BR.h)

//#define EFLETE_THEME "groupedit.edj"

/* here we create and define some functions for creating our groupedit smart
  object that is inherited from general Smart Object. */
EVAS_SMART_SUBCLASS_NEW(MY_CLASS_NAME, _groupedit,
                        Evas_Smart_Class, Evas_Smart_Class,
                        evas_object_smart_clipped_class_get, _smart_callbacks);

static Eina_Bool
_user_size_calc(Evas_Object *o)
{
   int nw, nh;
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(o, sd, EINA_FALSE)

   nw = sd->con_current_size->w + H_WIGTH;
   nh = sd->con_current_size->h + H_HEIGHT;

   evas_object_resize(o, nw, nh);

   return EINA_TRUE;
}

static void
_mouse_down_hTL_cb(void *data,
                   Evas *e,
                   Evas_Object *obj,
                   void *event_info)
{
   Evas_Event_Mouse_Down *ev = event_info;
   if (ev->button != 1) return;

   WS_GROUPEDIT_DATA_GET(data, sd)
   sd->downx = ev->canvas.x;
   sd->downy = ev->canvas.y;
   sd->handler_TL_pressed = EINA_TRUE;
   edje_object_signal_emit(sd->container, "tooltip,show", "eflete");
}

static void
_mouse_down_hRB_cb(void *data,
                   Evas *e,
                   Evas_Object *obj,
                   void *event_info)
{
   Evas_Event_Mouse_Down *ev = event_info;
   if (ev->button != 1) return;

   WS_GROUPEDIT_DATA_GET(data, sd)
   sd->downx = ev->canvas.x;
   sd->downy = ev->canvas.y;
   sd->handler_BR_pressed = EINA_TRUE;
   edje_object_signal_emit(sd->container, "tooltip,show", "eflete");
}

static void
_mouse_up_hTL_cb(void *data,
                 Evas *e,
                 Evas_Object *obj,
                 void *event_info)
{
   WS_GROUPEDIT_DATA_GET(data, sd)
   sd->handler_TL_pressed = EINA_FALSE;
   edje_object_signal_emit(sd->container, "tooltip,hide", "eflete");
}

static void
_mouse_up_hRB_cb(void *data,
                 Evas *e,
                 Evas_Object *obj,
                 void *event_info)
{
   WS_GROUPEDIT_DATA_GET(data, sd)
   sd->handler_BR_pressed = EINA_FALSE;
   edje_object_signal_emit(sd->container, "tooltip,hide", "eflete");
}

static void
_mouse_move_cb(void *data,
               Evas *e,
               Evas_Object *obj,
               void *event_info)
{
   Evas_Coord x, y, w, h;
   Evas_Coord lw, lh;
   Evas_Coord nx, ny, nw, nh;
   Evas_Coord dx, dy;
   Evas_Event_Mouse_Move *ev = event_info;
   Evas_Object *o = data;

   WS_GROUPEDIT_DATA_GET(o, sd)
   evas_object_geometry_get(o, &x, &y, &w, &h);
   evas_object_geometry_get(sd->container, NULL, NULL, &lw, &lh);
   dx = (ev->cur.canvas.x - sd->downx);
   dy = (ev->cur.canvas.y - sd->downy);

   /* check container size, if size equal min or max size,
      no sense to calc new geometry and render it */
   if (sd->handler_TL_pressed)
     {
        if ((dx < 0) && (dy < 0) &&
            (lw == sd->con_size_max.w) && (lh == sd->con_size_max.h))
          return;

        if ((dx > 0) && (dy > 0) &&
            (lw == sd->con_size_min.w) && (lh == sd->con_size_min.h))
          return;
     }

   if (sd->handler_BR_pressed)
     {
        if ((dx > 0) && (dy > 0) &&
            (lw == sd->con_size_max.w) && (lh == sd->con_size_max.h))
          return;

        if ((dx < 0) && (dy < 0) &&
            (lw == sd->con_size_min.w) && (lh == sd->con_size_min.h))
          return;
     }

   if (sd->handler_TL_pressed)
     {
        /* calc x and wigth */
        nw = w - dx;
        if (nw <= sd->con_size_min.w + H_WIGTH)
          {
             nw = sd->con_size_min.w + H_WIGTH;
             nx = x + (w - nw);
          }
        else
          {
             if ((sd->con_size_max.w != -1)
                 && (nw >= sd->con_size_max.w + H_WIGTH))
               {
                  nw = sd->con_size_max.w + H_WIGTH;
                  nx = x + (w - nw);
               }
             else nx = x + dx;
          }

        /* calc y and height */
        nh = h - dy;
        if (nh <= sd->con_size_min.h + H_HEIGHT)
          {
             nh = sd->con_size_min.h + H_HEIGHT;
             ny = y + (h - nh);
          }
        else
          {
             if ((sd->con_size_max.h != -1)
                 && (nh >= sd->con_size_max.h + H_HEIGHT))
               {
                  nh = sd->con_size_max.h + H_HEIGHT;
                  ny = y + (h - nh);
               }
             else ny = y + dy;
          }

        evas_object_resize(o, nw, nh);
        evas_object_move(o, nx, ny);

        evas_object_smart_changed(o);
     }
   if (sd->handler_BR_pressed)
     {
        /* calc wigth and heght */
        nw = w + dx;
        if (nw <= sd->con_size_min.w + H_WIGTH)
          nw = sd->con_size_min.w + H_WIGTH;
        else
          {
             if ((sd->con_size_max.w != -1)
                 && (nw >= sd->con_size_max.w + H_WIGTH))
               nw = sd->con_size_max.w + H_WIGTH;
             else nw = w + dx;
          }

        nh = h + dy;
        if (nh <= sd->con_size_min.h + H_HEIGHT)
          nh = sd->con_size_min.h + H_HEIGHT;
        else
          {
             if ((sd->con_size_max.h != -1)
                 && (nh >= sd->con_size_max.h + H_HEIGHT))
               nh = sd->con_size_max.h + H_HEIGHT;
             else nh = h + dy;
          }

        evas_object_resize(o, nw, nh);
        evas_object_move(o, x, y);

        evas_object_smart_changed(o);
     }
   sd->downx = ev->cur.canvas.x;
   sd->downy = ev->cur.canvas.y;
}

static void
_style_set(Evas_Object *o, const char *style)
{
   char group[512]; /* FIXME: change 512 to BUFF_MAX */
   WS_GROUPEDIT_DATA_GET(o, sd)

   #define GROUP_NAME(item, style) \
      sprintf(group, "eflete/groupedit/%s/%s", item, style);

   GROUP_NAME("base", style)
   if (!edje_object_file_set(sd->container, TET_EDJ, group))
     {
        GROUP_NAME("base", "default")
        edje_object_file_set(sd->container, TET_EDJ, group);
     }
   GROUP_NAME("handler_TL", style)
   if (!edje_object_file_set(sd->handler_TL.obj, TET_EDJ, group))
     {
        GROUP_NAME("handler_TL", "default")
        edje_object_file_set(sd->handler_TL.obj, TET_EDJ, group);
     }
   GROUP_NAME("handler_BR", style)
   if (!edje_object_file_set(sd->handler_BR.obj, TET_EDJ, group))
     {
        GROUP_NAME("handler_BR", style)
        edje_object_file_set(sd->handler_BR.obj, TET_EDJ, group);
     }

   if (sd->style) free((void *)sd->style);
   sd->style = strdup(group);

   #undef GROUP_NAME
}

/* create and setup a new example smart object's internals */
static void
_groupedit_smart_add(Evas_Object *o)
{
   EVAS_SMART_DATA_ALLOC(o, Ws_Groupedit_Smart_Data)

   _groupedit_parent_sc->add(o);

   priv->e = evas_object_evas_get(o);
   priv->container = edje_object_add(priv->e);

   priv->handler_TL.obj = edje_object_add(priv->e);
   priv->handler_TL.w = priv->handler_TL.h = 5;
   evas_object_event_callback_add(priv->handler_TL.obj, EVAS_CALLBACK_MOUSE_DOWN,
                                  _mouse_down_hTL_cb, o);
   evas_object_event_callback_add(priv->handler_TL.obj, EVAS_CALLBACK_MOUSE_UP,
                                  _mouse_up_hTL_cb, o);
   evas_object_event_callback_add(priv->handler_TL.obj, EVAS_CALLBACK_MOUSE_MOVE,
                                  _mouse_move_cb, o);

   priv->handler_BR.obj = edje_object_add(priv->e);
   priv->handler_BR.w = priv->handler_BR.h = 5;
   evas_object_event_callback_add(priv->handler_BR.obj, EVAS_CALLBACK_MOUSE_DOWN,
                                  _mouse_down_hRB_cb, o);
   evas_object_event_callback_add(priv->handler_BR.obj, EVAS_CALLBACK_MOUSE_UP,
                                  _mouse_up_hRB_cb, o);
   evas_object_event_callback_add(priv->handler_BR.obj, EVAS_CALLBACK_MOUSE_MOVE,
                                  _mouse_move_cb, o);

   priv->obj = o;
   priv->con_size_min.w = 0;
   priv->con_size_min.h = 0;
   priv->con_size_max.w = -1;
   priv->con_size_max.h = -1;
   priv->con_current_size = (Groupedit_Geom *)malloc(sizeof(Groupedit_Geom));
   //priv->con_current_size.w = 0;
   //priv->con_current_size.h = 0;
   priv->edit_obj = NULL;
   priv->parts = NULL;
   priv->handler_TL_pressed = EINA_FALSE;
   priv->handler_BR_pressed = EINA_FALSE;
   priv->obj_area.obj = NULL;
   priv->obj_area.gp = NULL;
   priv->obj_area.visible = EINA_FALSE;
   priv->separeted = EINA_FALSE;

   elm_config_cursor_engine_only_set(EINA_FALSE);
   elm_object_cursor_set(priv->handler_TL.obj, "top_left_corner");
   elm_object_cursor_set(priv->handler_BR.obj, "bottom_right_corner");
   elm_config_cursor_engine_only_set(EINA_TRUE);

   evas_object_smart_member_add(priv->container, o);
   evas_object_smart_member_add(priv->handler_TL.obj, o);
   evas_object_smart_member_add(priv->handler_BR.obj, o);

   _style_set(o, "default");
}

static void
_groupedit_smart_del(Evas_Object *o)
{
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(o, sd, RETURN_VOID)

   evas_object_smart_member_del(sd->container);
   evas_object_smart_member_del(sd->handler_TL.obj);
   evas_object_smart_member_del(sd->handler_BR.obj);

   _groupedit_parent_sc->del(o);
}

static void
_groupedit_smart_show(Evas_Object *o)
{
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(o, sd, RETURN_VOID);

   if (sd->handler_TL.obj) evas_object_show(sd->handler_TL.obj);
   if (sd->handler_BR.obj) evas_object_show(sd->handler_BR.obj);

   evas_object_show(sd->container);
   _groupedit_parent_sc->show(o);
}

static void
_groupedit_smart_hide(Evas_Object *o)
{
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(o, sd, RETURN_VOID)

   if (sd->handler_TL.obj) evas_object_hide(sd->handler_TL.obj);
   if (sd->handler_BR.obj) evas_object_hide(sd->handler_BR.obj);

   evas_object_hide(sd->container);
   _groupedit_parent_sc->hide(o);
}

static void
_groupedit_smart_resize(Evas_Object *o,
                            Evas_Coord w,
                            Evas_Coord h)
{
   Evas_Coord ow, oh;
   evas_object_geometry_get(o, NULL, NULL, &ow, &oh);
   if ((ow == w) && (oh == h)) return;

   evas_object_size_hint_min_set(o, w, h);
   evas_object_smart_changed(o);
}

static void
_groupedit_smart_calculate(Evas_Object *o)
{
   Evas_Coord x, y, w, h;
   Evas_Coord cw, ch;
   int htl_w, htl_h;
   int hrb_w, hrb_h;
   char buff[16];

   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(o, priv, RETURN_VOID)
   evas_object_geometry_get(o, &x, &y, &w, &h);
   htl_w = priv->handler_TL.w; htl_h = priv->handler_TL.h;
   hrb_w = priv->handler_BR.w; hrb_h = priv->handler_BR.h;

   cw = w - (htl_w + hrb_w);
   ch = h - (htl_h + hrb_h);
   evas_object_resize(priv->container, cw, ch);
   evas_object_move(priv->container, x + htl_w, y + htl_h);
   priv->con_current_size->x = x + htl_w;
   priv->con_current_size->y = y + htl_h;
   priv->con_current_size->w = cw;
   priv->con_current_size->h = ch;
   sprintf(buff, "%i %i", priv->con_current_size->w, priv->con_current_size->h);
   edje_object_part_text_set(priv->container, TEXT_TOOLTIP, buff);

   evas_object_resize(priv->handler_TL.obj, htl_w, htl_h);
   evas_object_move(priv->handler_TL.obj, x, y);

   evas_object_resize(priv->handler_BR.obj, hrb_w, hrb_h);
   evas_object_move(priv->handler_BR.obj, x + w - hrb_w, y + h - hrb_h);

   //Evas_Object *clipper = evas_object_smart_clipped_clipper_get(o);
   evas_object_resize(priv->edit_obj, cw, ch);
   evas_object_move(priv->edit_obj, x + htl_w, y + htl_h);
   //fprintf(stdout, "size: x[%i] y[%i] w[%i] h[%i]\n", x, y, w, h);

   _parts_recalc(priv);

   evas_object_smart_callback_call(o, SIG_CHANGED, (void *)priv->con_current_size);
}

/* this need for macro EVAS_SMART_SUBCLASS_NEW */
static void
_groupedit_smart_set_user(Evas_Smart_Class *sc)
{
   sc->add = _groupedit_smart_add;
   sc->del = _groupedit_smart_del;
   sc->show = _groupedit_smart_show;
   sc->hide = _groupedit_smart_hide;

   /* clipped smart object has no hook on resizes or calculations */
   sc->resize = _groupedit_smart_resize;
   sc->calculate = _groupedit_smart_calculate;
}

Evas_Object *
groupedit_add(Evas_Object *parent)
{
   Evas *e;
   Evas_Object *obj;

   if (!parent) return NULL;

   e = evas_object_evas_get(parent);
   obj = evas_object_smart_add(e, _groupedit_smart_class_new());

   return obj;
}

Eina_Bool
groupedit_handler_size_set(Evas_Object *obj, int htl_w, int htl_h, int hrb_w, int hrb_h)
{
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, EINA_FALSE);

   if (htl_w < 5) sd->handler_TL.w = 5;
   else sd->handler_TL.w = htl_w;
   if (htl_h < 5) sd->handler_TL.h = 5;
   else sd->handler_TL.h = htl_h;
   if (hrb_w < 5) sd->handler_BR.w = 5;
   else sd->handler_BR.w = hrb_w;
   if (hrb_h < 5) sd->handler_BR.h = 5;
   else sd->handler_BR.h = hrb_h;

   return EINA_TRUE;
}

Eina_Bool
groupedit_handler_size_get(Evas_Object *obj, int *htl_w, int *htl_h, int *hrb_w, int *hrb_h)
{
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, EINA_FALSE);

   *htl_w = sd->handler_TL.w;
   *htl_h = sd->handler_TL.h;
   *hrb_w = sd->handler_BR.w;
   *hrb_h = sd->handler_BR.h;

   return EINA_TRUE;
}

Eina_Bool
groupedit_min_size_set(Evas_Object *obj, int w, int h)
{
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, EINA_FALSE);

   if (w < 0) sd->con_size_min.w = 0;
   else sd->con_size_min.w = w;
   if (h < 0) sd->con_size_min.h = 0;
   else sd->con_size_min.h = h;

   return EINA_TRUE;
}

Eina_Bool
groupedit_max_size_set(Evas_Object *obj, int w, int h)
{
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, EINA_FALSE);

   if (w < 0) sd->con_size_max.w = -1;
   else sd->con_size_max.w = w;
   if (h < 0) sd->con_size_max.h = -1;
   else sd->con_size_max.h = h;

   return EINA_TRUE;
}

Eina_Bool
groupedit_container_size_set(Evas_Object *obj, int w, int h)
{
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, EINA_FALSE);

   if (w < sd->con_size_min.w) sd->con_current_size->w = sd->con_size_min.w;
   if ((sd->con_size_max.w != -1) && (w > sd->con_size_max.w))
     sd->con_current_size->w = sd->con_size_max.w;
   else sd->con_current_size->w = w;
   if (h < sd->con_size_min.h) sd->con_current_size->h = sd->con_size_min.h;
   if ((sd->con_size_max.h != -1) && (h > sd->con_size_max.h))
     sd->con_current_size->h = sd->con_size_max.h;
   else sd->con_current_size->h = h;

   return _user_size_calc(obj);
}

void
groupedit_container_size_get(Evas_Object *obj, int *w, int *h)
{
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, RETURN_VOID);

   *w = sd->con_current_size->w;
   *h = sd->con_current_size->h;
}

Eina_Bool
groupedit_style_set(Evas_Object *obj, const char *style)
{
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, EINA_FALSE);

   if (!style || (!strcmp(sd->style, style))) return EINA_FALSE;
   _style_set(obj, style);

   return EINA_TRUE;
}

const char *
groupedit_style_get(Evas_Object *obj)
{
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, NULL);
   return sd->style;
}

/******************************************************************************/
/******************************************************************************/

Eina_Bool
groupedit_edit_object_set(Evas_Object *obj,
                          Evas_Object *edit_obj,
                          const char *file)
{
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, EINA_FALSE);

   /* check input edit_obj, if it not a edje object return EINA_FALSE */
   if ((!edit_obj) && (!strcmp("edje", evas_object_type_get(edit_obj))))
     {
        /* TODO: add error message */
        return EINA_FALSE;
     }
   if (!file) return EINA_FALSE;

   edje_object_freeze(edit_obj);
   evas_object_hide(edit_obj);
   /*TODO: set the state for all parts to default 0.0 */
   sd->edit_obj = edit_obj;
   sd->edit_obj_file = file;
   sd->con_size_min.w = edje_edit_group_min_w_get(edit_obj);
   sd->con_size_min.h = edje_edit_group_min_h_get(edit_obj);
   /*  */
   if (edje_edit_group_max_w_get(edit_obj) == 0)
     sd->con_size_max.w = -1;
   else
     sd->con_size_max.w = edje_edit_group_max_w_get(edit_obj);
   if (edje_edit_group_max_h_get(edit_obj) == 0)
     sd->con_size_max.h = -1;
   else
     sd->con_size_max.h = edje_edit_group_max_h_get(edit_obj);
   //edje_object_part_swallow(sd->container, SWALLOW_FOR_EDIT, edit_obj);

   _edit_object_load(sd);
   if (sd->parts) _parts_hash_free(sd);
   _parts_hash_new(sd);

   evas_object_smart_changed(sd->obj);

   return EINA_TRUE;
}

Evas_Object *
groupedit_edit_object_unset(Evas_Object *obj)
{
   Evas_Object *ret;
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, NULL);

   if ((!sd->edit_obj) && (!sd->parts)) return NULL;

   _parts_hash_free(sd);
   ret = sd->edit_obj;
   sd->edit_obj = NULL;

   return ret;
}

void
groupedit_edit_object_recalc_all(Evas_Object *obj)
{
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, RETURN_VOID);

   _parts_recalc(sd);
}

Eina_Bool
groupedit_edit_object_part_state_set(Evas_Object *obj, const char *part,
                                     const char *state, double value)
{
   Eina_Bool ret;
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, false);

   ret = edje_edit_part_selected_state_set(sd->edit_obj, part, state, value);

   _parts_recalc(sd);
   return ret;
}

void
groupedit_part_object_area_set(Evas_Object *obj, const char *part)
{
   Groupspace_Part *gp;
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, RETURN_VOID);

   if (!sd->parts) return;

   gp = (Groupspace_Part *) eina_hash_find(sd->parts, part);
   sd->obj_area.gp = gp;
}

void
groupedit_part_object_area_visible_set(Evas_Object *obj, Eina_Bool visible)
{
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, RETURN_VOID);
   sd->obj_area.visible = visible;
}

Eina_Bool
groupedit_part_object_area_visible_get(Evas_Object *obj)
{
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, EINA_FALSE);
   return sd->obj_area.visible;
}

#undef MY_CLASS_NAME
#undef WS_GROUPSPACE_DATA_GET
#undef WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL
#undef H_WIGTH
#undef H_HEIGHT

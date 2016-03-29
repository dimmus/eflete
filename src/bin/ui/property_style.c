/*
 * Edje Theme Editor
 * Copyright (C) 2013-2014 Samsung Electronics.
 *
 * This file is part of Edje Theme Editor.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; If not, see www.gnu.org/licenses/lgpl.html.
 */

#define EO_BETA_API
#define EFL_BETA_API_SUPPORT
#define EFL_EO_API_SUPPORT

#include "property_private.h"
#include "property_macros.h"
#include "main_window.h"
#include "colorsel.h"
#include "project_manager.h"
#include "validator.h"

struct _Style_Prop_Data
{
   //Text Frame Data
   Evas_Object *font_name;
   Elm_Validator_Regexp *font_validator;
   Evas_Object *font_style;
   Evas_Object *font_color;
   Evas_Object *font_weight;
   Evas_Object *font_size;
   Evas_Object *box_frame_text;
   // Positioning option frame data
   Evas_Object *hor_align;
   Evas_Object *vert_align;
   Evas_Object *left_margin;
   Evas_Object *right_margin;
   Evas_Object *wrap;
   Evas_Object *box_frame_position;
   // Text format frame data
   Evas_Object *tab_stops;
   Evas_Object *line_size;
   Evas_Object *relative_size;
   Evas_Object *check_ellipsis;
   Evas_Object *replace_ellisis;
   Evas_Object *check_psw;
   Evas_Object *check_color;
   Evas_Object *bg_color;
   Evas_Object *box_format_frame;
   // Glow/Shadow frame data
   Evas_Object *style;
   Evas_Object *outer_gl_color;
   Evas_Object *inner_gl_color;
   Evas_Object *shadow_color;
   Evas_Object *direction;
   Evas_Object *box_glow_shadow;
   //Lines frame data
   Evas_Object *check_s_color;
   Evas_Object *s_color;
   Evas_Object *check_unline;
   Evas_Object *segment_ctrl;
   Evas_Object *outer_color_singl;
   Evas_Object *outer_color_double;
   Evas_Object *box_lines_frame;

   Style_Data current_style;
};
typedef struct _Style_Prop_Data Style_Prop_Data;

#define MIN_SP - 1.0
#define ELLIPSIS_MIN_SP 0.0
#define MARGIN_MIN_SP 0.0
#define MAX_SP 9999.0
#define MAX_PERCENT 100.0
#define STEP_SP 1.0
#define DIRECTION_NUM 39
#define WHITE_COLOR "#FFF"

#define DIRECTION_NUM 39
#define DEFAULT_DIRECTION 2

static const char *font_weight_list[] = { N_("Normal"),
                                          N_("Thin"),
                                          N_("Ultralight"),
                                          N_("Light"),
                                          N_("Book"),
                                          N_("Medium"),
                                          N_("Semibold"),
                                          N_("Bold"),
                                          N_("Ultrabold"),
                                          N_("Black"),
                                          N_("Extrablack"),
                                          NULL};

static const char *font_width_list[] = { N_("Normal"),
                                         N_("Ultracondensed"),
                                         N_("Extracondensed"),
                                         N_("Condensed"),
                                         N_("Extracondensed"),
                                         N_("Semicondensed"),
                                         N_("Semiexpanded"),
                                         N_("Expanded"),
                                         N_("Ultraexpanded"),
                                         NULL};

static const char *style_table[][2] = {{"font", NULL},
                                       {"font_fallback", NULL},
                                       {"font_size", NULL},
                                       {"font_source", NULL},
                                       {"font_weight", NULL},
                                       {"font_style", NULL},
                                       {"font_width", NULL},
                                       {"lang", NULL},
                                       {"color", NULL},
                                       {"underline_color", NULL},
                                       {"underline2_color", NULL},
                                       {"underline_dash_color", NULL},
                                       {"outline_color", NULL},
                                       {"shadow_color", NULL},
                                       {"glow_color", NULL},
                                       {"glow2_color", NULL},
                                       {"backing_color", NULL},
                                       {"strikethrough_color", NULL},
                                       {"align", NULL},
                                       {"valign", NULL},
                                       {"wrap", NULL},
                                       {"left_margin", NULL},
                                       {"right_margin", NULL},
                                       {"underline", NULL},
                                       {"strikethrough", NULL},
                                       {"backing", NULL},
                                       {"style", NULL},
                                       {"tabstops", NULL},
                                       {"linesize", NULL},
                                       {"linerelsize", NULL},
                                       {"linegap", NULL},
                                       {"linerelgap", NULL},
                                       {"item", NULL},
                                       {"linefill", NULL},
                                       {"ellipsis", NULL},
                                       {"password", NULL},
                                       {"underline_dash_width", NULL},
                                       {"underline_dash_gap", NULL},
                                       {NULL, NULL},
                                       {"direction", NULL}};

static const char *underl_styles[] = { N_("single"),
                                       N_("double"),
                                       NULL};

static const char *font_horizontal_align[] = { N_("auto"),
                                               N_("center"),
                                               N_("middle"),
                                               N_("left"),
                                               N_("right"),
                                               NULL};

static const char *font_horizontal_valign[] = { N_("top"),
                                                N_("bottom"),
                                                N_("middle"),
                                                N_("center"),
                                                N_("baseline"),
                                                N_("base"),
                                                NULL};

static const char *text_wrap[] = { N_("none"),
                                   N_("word"),
                                   N_("char"),
                                   N_("mixed"),
                                   NULL};

static const char *font_glow_list[] = { N_("none"),
                                        N_("plain"),
                                        N_("shadow"),
                                        N_("outline"),
                                        N_("soft_outline"),
                                        N_("outline_shadow"),
                                        N_("outline_soft_shadow"),
                                        N_("glow"),
                                        N_("shadow"),
                                        N_("far_shadow"),
                                        N_("soft_shadow"),
                                        N_("far_soft_shadow"),
                                        NULL};

static const char *direction_list[] = { N_("bottom_left"),
                                        N_("bottom"),
                                        N_("bottom_right"),
                                        N_("left"),
                                        N_("right"),
                                        N_("top_left"),
                                        N_("top"),
                                        N_("top_right"),
                                        NULL};

static void _glow_shadow_prop_update(Style_Prop_Data *, const char *);

static void _lines_prop_update(Style_Prop_Data *, const char *);

#define FONT_DEFAULT "DEFAULT='align=middle font=Sans font_size=24 color=#000000 "
#define STYLE_PROP_DATA "style_prop_data"

#define CURRENT pd->current_style

#define STYLE_PROP_DATA_GET() \
   assert(property != NULL); \
   Style_Prop_Data *pd = evas_object_data_get(property, STYLE_PROP_DATA); \
   assert(pd != NULL);

#define ADD_1SWALLOW_ITEM(TEXT, BOX, OBJECT, ITEM, TYPE_OBJECT) \
{ \
   PROPERTY_ITEM_ADD(BOX, TEXT, "1swallow"); \
   TYPE_OBJECT##_ADD(BOX, OBJECT); \
   elm_layout_content_set(ITEM, "elm.swallow.content", OBJECT); \
   elm_box_pack_end(BOX, ITEM); \
}

#define ADD_COLOR_ELEMENT(OBJECT, ITEM, LAYOUT) \
   OBJECT = elm_layout_add(ITEM); \
   elm_layout_theme_set(OBJECT, "layout", "property", "color"); \
   evas_object_size_hint_weight_set(OBJECT,  EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   evas_object_size_hint_align_set(OBJECT, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   LAYOUT = elm_layout_add(ITEM); \
   elm_layout_theme_set(LAYOUT, "image", "color", "color_set"); \
   elm_layout_content_set(OBJECT, NULL, LAYOUT);

#define SPINNER_VALUE \
const char *value = NULL; \
if (!ei) \
  { \
     value = eina_stringshare_printf("%f", elm_spinner_value_get(obj)); \
  }

#define COMBOBOX_VALUE \
Ewe_Combobox_Item *item = ei; \
const char *value; \
value = eina_stringshare_add(item->title);

#define COMBOBOX_NONE_VALUE \
Ewe_Combobox_Item *item = ei; \
const char *value; \
value = eina_stringshare_add(item->title); \
if (!strcmp(value, _("none"))) \
{ \
   eina_stringshare_del(value); \
   value = eina_stringshare_add(""); \
}

#define PERCENT_SPINNER_VALUE \
const char *value = NULL; \
if (!ei) \
  { \
     value = eina_stringshare_printf("%f", elm_spinner_value_get(obj)/100); \
  }

#define CHECK_VALUE \
Evas_Object *check = obj; \
const char *value = NULL; \
if (elm_check_state_get(check)) value = eina_stringshare_add("on"); \
else value = eina_stringshare_add("off");

#define SEGMENT_VALUE \
Elm_Object_Item *item = ei; \
const char *value; \
value = eina_stringshare_add(elm_object_item_part_text_get(item, "elm.text"));

#define FONT_VALUE \
Eina_Stringshare *value; \
if (elm_validator_regexp_status_get(pd->font_validator) != ELM_REG_NOERROR) \
  return; \
value = eina_stringshare_add(elm_entry_entry_get(obj));

#define CHANGE_CALLBACK(VALUE, TEXT, WIDGET, FUNC) \
static void \
_on_##VALUE##_change(void *data, \
                     Evas_Object *obj EINA_UNUSED, \
                     void *ei EINA_UNUSED) \
{ \
   Style_Prop_Data *pd = (Style_Prop_Data *)data; \
   assert(pd != NULL); \
   WIDGET##_VALUE \
   _tag_parse(pd, value, TEXT); \
   FUNC; \
   _lines_colors_update(pd, TEXT); \
   _style_edit_update(pd); \
   eina_stringshare_del(value); \
   editor_save(ap.project->global_object); \
   TODO("Remove this line once edje_edit API would be added into Editor Module and saving would work properly") \
   ap.project->changed = true; \
}

static void
_entry_repch_update(Style_Prop_Data *pd, Eina_Bool password)
{
   if (password)
     evas_object_textblock_replace_char_set(CURRENT.textblock_style, "*");
   else
     evas_object_textblock_replace_char_set(CURRENT.textblock_style, NULL);
}

static Eina_Bool
_hex_to_rgb(const char *hex, int *r, int *g, int *b, int *a)
{
   assert(hex != NULL);
   assert(r != NULL);
   assert(g != NULL);
   assert(b != NULL);
   assert(a != NULL);

   unsigned long val;
   char *end;
   int length = (int)strlen(hex);
   switch (length)
     {
      case 7:
        {
           val = strtoul(hex + 1, &end, 16);
           *r = (val & 0xff0000) >> 16;
           *g = (val & 0xff00) >> 8;
           *b = val & 0xff;
           *a = 255;
           break;
        }
      case 9:
        {
           val = strtoul(hex + 1, &end, 16);
           *r = (val & 0xff000000) >> 24;
           *g = (val & 0xff0000) >> 16;
           *b = (val & 0xff00) >> 8;
           *a = val & 0xff;
           break;
        }
      case 5:
        {
           val = strtoul(hex + 1, &end, 16);
           *r = (val & 0xf000) >> 12;
           *g = (val & 0xf00) >> 8;
           *b = (val & 0xf0) >> 4;
           *a = val & 0xf;
           *r = (*r << 4) | *r;
           *g = (*g << 4) | *g;
           *b = (*b << 4) | *b;
           *a = (*a << 4) | *a;
           break;
        }
      case 4:
        {
           val = strtoul(hex + 1, &end, 16);
           *r = (val & 0xf00) >> 8;
           *g = (val & 0xf0) >> 4;
           *b = val & 0xf;
           *a = 255;
           *r = (*r << 4) | *r;
           *g = (*g << 4) | *g;
           *b = (*b << 4) | *b;
           break;
        }
      default:
        {
           *r = 0;
           *g = 0;
           *b = 0;
           *a = 255;
        }
     }
   return true;
}

static void
_tag_parse(Style_Prop_Data *pd, const char *value, const char *text)
{
   Evas_Object *edje_edit_obj = NULL;
   Eina_Strbuf *tag = eina_strbuf_new();
   char *stolen_buf;
   char *token;
   int i = 0, k = 0, exist = 0, style_length = 0;

   assert(pd != NULL);
   assert(value != NULL);
   assert(text != NULL);

   eina_strbuf_append(tag, CURRENT.stvalue);
   edje_edit_obj = ap.project->global_object;
   stolen_buf = eina_strbuf_string_steal(tag);

   token = strtok(stolen_buf, " =+");
   while (token)
     {
        if ((i + 1) % 2 != 0)
          {
             for (k = 0; style_table[k][0] != NULL; k++)
               {
                  if (!strcmp(style_table[k][0], token)) exist = k;
               }
          }
        else if (strstr(token, "shadow"))
          {
             style_table[DIRECTION_NUM][1] = eina_stringshare_add(strchr(token, ','));
             if (style_table[DIRECTION_NUM][1])
               {
                  style_length = (int)(strlen(token) - strlen(style_table[DIRECTION_NUM][1]));
                  style_table[exist][1] = eina_stringshare_add_length(token, style_length);
               }
          }
        else
          {
             style_table[exist][1] = eina_stringshare_add(token);
          }
        token= strtok(NULL, " =+");
        i++;
     }
   free(stolen_buf);
   if (!strcmp(text, "password"))
     _entry_repch_update(pd, !strcmp(value, "on"));

   if (!strcmp(text, "direction"))
     {
        if (style_table[DIRECTION_NUM][1]) eina_stringshare_del(style_table[DIRECTION_NUM][1]);
        style_table[DIRECTION_NUM][1] = eina_stringshare_printf(",%s", value);
     }
   else
     {
        for (k = 0; style_table[k][0] != NULL; k++)
          {
             if (!strcmp(style_table[k][0], text))
               {
                  eina_stringshare_del(style_table[k][1]);
                  style_table[k][1] = eina_stringshare_add(value);
               }
          }
     }
   if ((!strcmp(text, "style")) && (!style_table[DIRECTION_NUM][1]))
     style_table[DIRECTION_NUM][1] = eina_stringshare_add(",bottom_right");
   eina_strbuf_append(tag, "+ ");
   for (k = 0; style_table[k][0] != NULL; k++)
      {
         if ((style_table[k][1] != NULL) && (!strstr(style_table[k][1], "shadow")))
           {
              eina_strbuf_append(tag, style_table[k][0]);
              eina_strbuf_append(tag, "=");
              eina_strbuf_append(tag, style_table[k][1]);
              eina_strbuf_append(tag, " ");
              eina_stringshare_del(style_table[k][1]);
              style_table[k][1] = NULL;
           }
         else if ((style_table[k][1] != NULL) && (strstr(style_table[k][1], "shadow")))
           {
              eina_strbuf_append(tag, style_table[k][0]);
              eina_strbuf_append(tag, "=");
              eina_strbuf_append(tag, style_table[k][1]);
              eina_strbuf_append(tag, style_table[DIRECTION_NUM][1]);
              eina_strbuf_append(tag, " ");
              eina_stringshare_del(style_table[k][1]);
              style_table[k][1] = NULL;
           }
      }
   edje_edit_style_tag_value_set(edje_edit_obj, CURRENT.st_name,
                                 CURRENT.st_tag, eina_strbuf_string_get(tag));
   eina_stringshare_del(CURRENT.stvalue);
   CURRENT.stvalue = eina_stringshare_add(eina_strbuf_string_get(tag));
   eina_strbuf_free(tag);
}

static Eina_Tmpstr*
_tag_value_get(const char* text_style, char* a_tag)
{
   assert(text_style != NULL);
   assert(a_tag != NULL);

   char *tag_list_copy = mem_malloc(strlen(text_style) + 1);
   Eina_Tmpstr *result = NULL;
   char *token;

   strcpy(tag_list_copy, text_style);
   token = strtok(tag_list_copy, " ");

   while (token)
     {
        char* equals_sign = strchr(token, '=');
        if (equals_sign)
          {
             *equals_sign = 0;
             if (!strcmp(token, a_tag))
               {
                  equals_sign++;

                  if (equals_sign[strlen(equals_sign) - 1] != '\'')
                    result = eina_tmpstr_add(equals_sign);
                  else
                    result = eina_tmpstr_add_length(equals_sign, strlen(equals_sign) - 1);
                  if (!strstr(FONT_DEFAULT, a_tag)) break;
               }
          }
        token = strtok(0, " ");
     }
   free(tag_list_copy);

   return result;
}

#define TAB_UPDATE(VALUE) \
static void \
_##VALUE##_update(Style_Prop_Data *pd) \
{ \
   assert(pd != NULL); \
   _##VALUE##_prop_update(pd, CURRENT.stvalue); \
}

TAB_UPDATE(lines)

TAB_UPDATE(glow_shadow)

static void
_lines_colors_update(Style_Prop_Data *pd, const char *param)
{
   Eina_Tmpstr *val;

   assert(pd != NULL);
   assert(param != NULL);

   if (!strcmp(param, "underline"))
     {
        _lines_update(pd);
        val = _tag_value_get(CURRENT.stvalue, "underline_color");
        if (!val) _tag_parse(pd, WHITE_COLOR, "underline_color");
        else eina_tmpstr_del(val);

        val = _tag_value_get(CURRENT.stvalue, "underline2_color");
        if (!val) _tag_parse(pd, WHITE_COLOR, "underline2_color");
        else eina_tmpstr_del(val);
     }
   else if (!strcmp(param, "strikethrough"))
     {
        val = _tag_value_get(CURRENT.stvalue, "strikethrough_color");
        if (!val) _tag_parse(pd, WHITE_COLOR, "strikethrough_color");
        else eina_tmpstr_del(val);
     }
}

static void
_style_edit_update(Style_Prop_Data *pd)
{
   Evas_Textblock_Style *ts = NULL;
   Eina_Strbuf *style = eina_strbuf_new();

   assert(pd != NULL);

   eina_strbuf_append(style, FONT_DEFAULT);
   eina_strbuf_append(style, CURRENT.stvalue);
   eina_strbuf_append(style, "'");
   ts = evas_textblock_style_new();
   evas_textblock_style_set(ts, eina_strbuf_string_get(style));
   evas_object_textblock_style_set(CURRENT.textblock_style, ts);
   evas_object_size_hint_max_set(CURRENT.textblock_style, EVAS_HINT_FILL,
                                 EVAS_HINT_FILL);
   eina_strbuf_free(style);
   evas_textblock_style_free(ts);
}

static void
_prop_text_color_add(const char *text, Evas_Object *box, Evas_Object **object)
{
   Evas_Object *layout;
   PROPERTY_ITEM_ADD(box, text, "1swallow_subtext")
   ADD_COLOR_ELEMENT(*object, item, layout)
   elm_layout_content_set(item, NULL, *object);
   elm_box_pack_end(box, item);
}

static void
_prop_text_2color_add(const char *text,
                      Evas_Object *box,
                      Evas_Object **object_left,
                      Evas_Object **object_right)
{
   Evas_Object *layout;
   PROPERTY_ITEM_ADD(box, text, "2swallow")

   ADD_COLOR_ELEMENT(*object_left, item, layout)
   ADD_COLOR_ELEMENT(*object_right, item, layout)

   elm_layout_content_set(item, "swallow.content1", *object_left);
   elm_layout_content_set(item, "swallow.content2", *object_right);

   elm_box_pack_end(box, item);
}

static void
_prop_spin_add(const char *text,
               const char *LS,
               const char *RS,
               Evas_Object *box,
               Evas_Object **object,
               float min,
               float max,
               float step)
{
   PROPERTY_ITEM_ADD(box, text, "2swallow")
   SPINNER_ADD(item, *object, min, max, step, true)

   elm_layout_content_set(item, "swallow.content1", *object);
   elm_layout_text_set(item, "label.swallow1.start", LS);
   elm_layout_text_set(item, "label.swallow1.end", RS);

   elm_box_pack_end(box, item);
}

static void
_add_1check_color_item(const char *text,
                       Evas_Object *box,
                       Evas_Object **object_left,
                       Evas_Object **object_right)
{
   Evas_Object *layout;
   PROPERTY_ITEM_ADD(box, text, "2swallow")
   CHECK_ADD(item, *object_left)
   elm_object_style_set(*object_left, "toggle");

   ADD_COLOR_ELEMENT(*object_right, item, layout)

   elm_layout_content_set(item, "swallow.content1", *object_left);
   elm_layout_content_set(item, "swallow.content2", *object_right);

   elm_box_pack_end(box, item);
}

static void
_add_1check_2button_item(const char *text,
                         Evas_Object *box,
                         Evas_Object **object_left,
                         Evas_Object **object_right)
{
   PROPERTY_ITEM_ADD(box, text, "2swallow")
   CHECK_ADD(item, *object_left)
   elm_object_style_set(*object_left, "toggle");

   SEGMENT_CONTROL_ADD(item, *object_right)
   elm_segment_control_item_add(*object_right, NULL, "single");
   elm_segment_control_item_add(*object_right, NULL, "double");

   elm_layout_content_set(item, "swallow.content1", *object_left);
   elm_layout_content_set(item, "swallow.content2", *object_right);

   elm_box_pack_end(box, item);
}

static void
_add_1check_spinner_item(const char *text,
                         Evas_Object *box,
                         Evas_Object **object_left,
                         Evas_Object **object_right)
{
   PROPERTY_ITEM_ADD(box, text, "2swallow")
   CHECK_ADD(item, *object_left)
   elm_object_style_set(*object_left, "toggle");

   SPINNER_ADD(item, *object_right, ELLIPSIS_MIN_SP, MAX_SP, STEP_SP, true)

   elm_layout_content_set(item, "swallow.content1", *object_left);
   elm_layout_content_set(item, "swallow.content2", *object_right);

   elm_box_pack_end(box, item);
}

#define CALLBACKS_COLOR_ADD(VALUE, TAG) \
static void \
_on_##VALUE##_change(void *data, \
                     Evas_Object *obj, \
                     void *ei __UNUSED__) \
{ \
   int r, g, b, a; \
   const char *value; \
   Style_Prop_Data *pd = (Style_Prop_Data *)data; \
   assert(pd != NULL); \
   Evas_Object *color; \
   color = evas_object_data_get(obj, "color"); \
   colorselector_color_get(obj, &r, &g, &b, &a); \
   value = eina_stringshare_printf("#%02x%02x%02x%02x", r, g, b, a); \
   _tag_parse(pd, value, TAG); \
   evas_object_color_set(color, r*a/255, g*a/255, b*a/255, a); \
   _style_edit_update(pd); \
   eina_stringshare_del(value); \
   editor_save(ap.project->global_object); \
   TODO("Remove this line once edje_edit API would be added into Editor Module and saving would work properly") \
   ap.project->changed = true; \
} \
static void \
_on_##VALUE##_dismissed(void *data, \
                        Evas_Object *obj, \
                        void *event_info __UNUSED__) \
{ \
   Style_Prop_Data *pd = (Style_Prop_Data *)data; \
   assert(pd != NULL); \
   evas_object_smart_callback_del_full(obj, "color,changed", \
                                      _on_##VALUE##_change, pd); \
   evas_object_smart_callback_del_full(obj, "palette,item,selected", \
                                       _on_##VALUE##_change, pd); \
   evas_object_data_del(obj, "color"); \
   evas_object_hide(obj); \
} \
static void \
_on_##VALUE##_clicked(void *data, \
                      Evas *e __UNUSED__, \
                      Evas_Object *obj, \
                      void *event_info __UNUSED__) \
{ \
   int x, y; \
   int r, g, b, a; \
   Style_Prop_Data *pd = (Style_Prop_Data *)data; \
   assert(pd != NULL); \
   Evas_Object *colorsel; \
   colorsel = colorselector_get(); \
   evas_object_data_set(colorsel, "color", obj); \
   evas_object_color_get(obj, &r, &g, &b, &a); \
   colorselector_color_set(colorsel, r, g, b, a); \
   evas_object_smart_callback_add(colorsel, "color,changed", \
                                  _on_##VALUE##_change, pd); \
   evas_object_smart_callback_add(colorsel, "palette,item,selected", \
                                  _on_##VALUE##_change, pd); \
   evas_object_smart_callback_add(colorsel, "dismissed", \
                                  _on_##VALUE##_dismissed, pd); \
   evas_pointer_canvas_xy_get(evas_object_evas_get(obj), &x, &y); \
   evas_object_move(colorsel, x, y); \
   evas_object_show(colorsel); \
}

CALLBACKS_COLOR_ADD(font_color, "color")
CALLBACKS_COLOR_ADD(bg_color, "backing_color")
CALLBACKS_COLOR_ADD(inner_gl_color, "glow_color")
CALLBACKS_COLOR_ADD(outer_gl_color, "glow2_color")
CALLBACKS_COLOR_ADD(shadow_color, "shadow_color")
CALLBACKS_COLOR_ADD(s_color, "strikethrough_color")
CALLBACKS_COLOR_ADD(outer_color_singl, "underline_color")
CALLBACKS_COLOR_ADD(outer_color_double, "underline2_color")

CHANGE_CALLBACK(font_name, "font", FONT, NULL)
CHANGE_CALLBACK(font_size, "font_size", SPINNER, NULL)
CHANGE_CALLBACK(font_style, "font_width", COMBOBOX, NULL)
CHANGE_CALLBACK(font_weight, "font_weight", COMBOBOX, NULL)

CHANGE_CALLBACK(tab_stops, "tabstops", SPINNER, NULL)
CHANGE_CALLBACK(line_size, "linesize", SPINNER, NULL)
CHANGE_CALLBACK(relative_size, "linerelsize", SPINNER, NULL)
CHANGE_CALLBACK(check_psw, "password", CHECK, NULL)
CHANGE_CALLBACK(check_color, "backing", CHECK, NULL)
CHANGE_CALLBACK(replace_ellisis, "ellipsis", PERCENT_SPINNER, NULL)
static void
_on_check_ellipsis_change(void *data,
                          Evas_Object *obj EINA_UNUSED,
                          void *ei EINA_UNUSED)
{
   Style_Prop_Data *pd = (Style_Prop_Data *)data;
   assert(pd != NULL);
   Eina_Stringshare *value;
   if (!elm_check_state_get(obj))
     {
        value = eina_stringshare_add("-1.0");
        elm_object_disabled_set(pd->replace_ellisis, true);
     }
   else
     {
        value = eina_stringshare_add("0.0");
        elm_object_disabled_set(pd->replace_ellisis, false);
     }

   elm_spinner_value_set(pd->replace_ellisis, 0.0);
   _tag_parse(pd, value, "ellipsis");

   _lines_colors_update(pd, "ellipsis");
   _style_edit_update(pd);
   eina_stringshare_del(value);
}

CHANGE_CALLBACK(hor_align, "align", COMBOBOX, NULL)
CHANGE_CALLBACK(left_margin, "left_margin", SPINNER, NULL)
CHANGE_CALLBACK(vert_align, "valign", COMBOBOX, NULL)
CHANGE_CALLBACK(right_margin, "right_margin", SPINNER, NULL)
CHANGE_CALLBACK(wrap, "wrap", COMBOBOX_NONE, NULL)
CHANGE_CALLBACK(style, "style", COMBOBOX, _glow_shadow_update(pd))

CHANGE_CALLBACK(check_s_color, "strikethrough", CHECK, NULL)
CHANGE_CALLBACK(check_unline, "underline", CHECK, NULL)
CHANGE_CALLBACK(segment_ctrl, "underline", SEGMENT, NULL)

static void
_add_text_part(Style_Prop_Data *pd)
{
   /* unnamed block to suppress "error: declaration of 'item' shadows a previous local" */
     {
        PROPERTY_ITEM_ADD(pd->box_frame_text, _("font name"), "1swallow");
        ENTRY_ADD(pd->box_frame_text, pd->font_name, true);
        elm_layout_content_set(item, "elm.swallow.content", pd->font_name);
        elm_box_pack_end(pd->box_frame_text, item);
        evas_object_smart_callback_add(pd->font_name, "changed,user", _on_font_name_change, pd);
        if (pd->font_validator == NULL)
          pd->font_validator = elm_validator_regexp_new(FONT_STYLE_REGEX, NULL);
        eo_event_callback_add(pd->font_name, ELM_ENTRY_EVENT_VALIDATE, elm_validator_regexp_helper, pd->font_validator);
     }

   ADD_1SWALLOW_ITEM(_("font style"), pd->box_frame_text, pd->font_style, item, EWE_COMBOBOX);
   evas_object_smart_callback_add(pd->font_style, "selected", _on_font_style_change, pd);
   ADD_1SWALLOW_ITEM(_(""), pd->box_frame_text, pd->font_weight, item, EWE_COMBOBOX);
   evas_object_smart_callback_add(pd->font_weight, "selected", _on_font_weight_change, pd);

   _prop_text_color_add(_("color"), pd->box_frame_text, &pd->font_color);
   evas_object_event_callback_add(pd->font_color, EVAS_CALLBACK_MOUSE_DOWN,
                                  _on_font_color_clicked, pd);

   _prop_spin_add(_("size"), _(""), _("px"),
                  pd->box_frame_text, &pd->font_size, MIN_SP, MAX_SP, STEP_SP);

   evas_object_smart_callback_add(pd->font_size, "changed", _on_font_size_change, pd);
}

static Evas_Object *
_add_text_frame(Style_Prop_Data *pd, Evas_Object *parent)
{
   Evas_Object *frame_text;

   FRAME_ADD(parent, frame_text, true, _("Text"))

   BOX_ADD(frame_text, pd->box_frame_text, EINA_FALSE, EINA_FALSE);
   elm_box_padding_set(pd->box_frame_text, 6, 0);
   elm_object_content_set(frame_text, pd->box_frame_text);

   return frame_text;
}

static void
_add_position_part(Style_Prop_Data *pd)
{
   ADD_1SWALLOW_ITEM(_("horizontal align"), pd->box_frame_position, pd->hor_align, item, EWE_COMBOBOX);
   evas_object_smart_callback_add(pd->hor_align, "selected", _on_hor_align_change, pd);

   ADD_1SWALLOW_ITEM(_("vertical align"), pd->box_frame_position, pd->vert_align, item, EWE_COMBOBOX);
   evas_object_smart_callback_add(pd->vert_align, "selected", _on_vert_align_change, pd);

   _prop_spin_add(_("margin"), _("left"), _("px"), pd->box_frame_position, &pd->left_margin, MARGIN_MIN_SP, MAX_SP, STEP_SP);
   evas_object_smart_callback_add(pd->left_margin, "changed", _on_left_margin_change, pd);

   _prop_spin_add(_(""), _("right"), _("px"), pd->box_frame_position, &pd->right_margin, MARGIN_MIN_SP, MAX_SP, STEP_SP);
   evas_object_smart_callback_add(pd->right_margin, "changed", _on_right_margin_change, pd);

   ADD_1SWALLOW_ITEM(_("wrap"), pd->box_frame_position, pd->wrap, item, EWE_COMBOBOX);
   evas_object_smart_callback_add(pd->wrap, "selected", _on_wrap_change, pd);
}

static Evas_Object *
_add_position_frame(Style_Prop_Data *pd, Evas_Object *parent)
{
   Evas_Object *frame_position;

   FRAME_ADD(parent, frame_position, true, _("Positioning option"))

   BOX_ADD(frame_position, pd->box_frame_position, EINA_FALSE, EINA_FALSE);
   elm_box_padding_set(pd->box_frame_position, 6, 0);
   elm_object_content_set(frame_position, pd->box_frame_position);

   return frame_position;
}

static void
_add_text_format_part(Style_Prop_Data *pd)
{
   _prop_spin_add(_("text tabstops"), _(""), _("px"), pd->box_format_frame, &pd->tab_stops, MIN_SP, MAX_SP, STEP_SP);
   evas_object_smart_callback_add(pd->tab_stops, "changed", _on_tab_stops_change, pd);

   _prop_spin_add(_("line size"), _(""), _("px"), pd->box_format_frame, &pd->line_size, MIN_SP, MAX_SP, STEP_SP);
   evas_object_smart_callback_add(pd->line_size, "changed", _on_line_size_change, pd);

   _add_1check_color_item(_("background color"), pd->box_format_frame, &pd->check_color, &pd->bg_color);
   evas_object_smart_callback_add(pd->check_color, "changed", _on_check_color_change, pd);
   evas_object_event_callback_add(pd->bg_color, EVAS_CALLBACK_MOUSE_DOWN,
                                  _on_bg_color_clicked, pd);

   ADD_1SWALLOW_ITEM(_("password field"), pd->box_format_frame, pd->check_psw, item, CHECK);
   evas_object_smart_callback_add(pd->check_psw, "changed", _on_check_psw_change, pd);

   _add_1check_spinner_item(_("ellisis"), pd->box_format_frame, &pd->check_ellipsis, &pd->replace_ellisis);
   evas_object_smart_callback_add(pd->replace_ellisis, "changed", _on_replace_ellisis_change, pd);
   evas_object_smart_callback_add(pd->check_ellipsis, "changed", _on_check_ellipsis_change, pd);

   _prop_spin_add(_("line relative size"), _(""), _("px"), pd->box_format_frame, &pd->relative_size, MIN_SP, MAX_SP, STEP_SP);
   evas_object_smart_callback_add(pd->relative_size, "changed", _on_relative_size_change, pd);
}

static Evas_Object *
_add_text_format_frame(Style_Prop_Data *pd, Evas_Object *parent)
{
   Evas_Object *frame_text_format;

   FRAME_ADD(parent, frame_text_format, true, _("Text format"))

   BOX_ADD(frame_text_format, pd->box_format_frame, EINA_FALSE, EINA_FALSE);
   elm_box_padding_set(pd->box_format_frame, 6, 0);
   elm_object_content_set(frame_text_format, pd->box_format_frame);

   return frame_text_format;
}

static Evas_Object *
_add_glow_shadow_frame(Style_Prop_Data *pd, Evas_Object *parent)
{
   Evas_Object *frame_glow;

   FRAME_ADD(parent, frame_glow, true, _("Glow & Shadow"))

   BOX_ADD(frame_glow, pd->box_glow_shadow, EINA_FALSE, EINA_FALSE);
   elm_object_content_set(frame_glow, pd->box_glow_shadow);

   elm_box_padding_set(pd->box_glow_shadow, 6, 0);

   return frame_glow;
}

static void
_add_inner_outer_part(Style_Prop_Data *pd)
{
   ADD_1SWALLOW_ITEM(_("style"), pd->box_glow_shadow, pd->style, item, EWE_COMBOBOX);
   evas_object_smart_callback_add(pd->style, "selected", _on_style_change, pd);

   _prop_text_color_add(_("outer glow color"), pd->box_glow_shadow, &pd->outer_gl_color);
   evas_object_event_callback_add(pd->outer_gl_color, EVAS_CALLBACK_MOUSE_DOWN,
                                  _on_outer_gl_color_clicked, pd);

   _prop_text_color_add(_("inner glow color"), pd->box_glow_shadow, &pd->inner_gl_color);
   evas_object_event_callback_add(pd->inner_gl_color, EVAS_CALLBACK_MOUSE_DOWN,
                                  _on_inner_gl_color_clicked, pd);
}

static void
_on_select_direction(void *data,
                     Evas_Object *obj __UNUSED__,
                     void *event_info)
{
   Ewe_Combobox_Item *it = event_info;
   Style_Prop_Data *pd = data;

   assert(pd != NULL);

   eina_stringshare_del(style_table[DIRECTION_NUM][1]);
   style_table[DIRECTION_NUM][1] = eina_stringshare_add(it->title);
   _tag_parse(pd, it->title, "direction");
   _style_edit_update(pd);
}

static Evas_Object *
_add_direction_item(Evas_Object *parent, Style_Prop_Data *pd)
{
   int i;

   pd->direction = ewe_combobox_add(parent);
   evas_object_smart_callback_add(pd->direction, "selected", _on_select_direction, pd);
   evas_object_show(pd->direction);
   PROPERTY_ITEM_ADD(parent, _("direction"), "1swallow")
   elm_layout_content_set(item, NULL, pd->direction);
   elm_box_pack_end(parent, item);
   for(i = 0; direction_list[i] != NULL; i++)
     ewe_combobox_item_add(pd->direction, direction_list[i]);

   return pd->direction;
}

static Evas_Object *
_add_shadow_part(Style_Prop_Data *pd)
{
   ADD_1SWALLOW_ITEM(_("style"), pd->box_glow_shadow, pd->style, item, EWE_COMBOBOX);
   evas_object_smart_callback_add(pd->style, "selected", _on_style_change, pd);

   _prop_text_color_add(_("color"), pd->box_glow_shadow, &pd->shadow_color);
   evas_object_event_callback_add(pd->shadow_color, EVAS_CALLBACK_MOUSE_DOWN,
                                  _on_shadow_color_clicked, pd);

   return _add_direction_item(pd->box_glow_shadow, pd);
}

static void
_add_lines_part(Style_Prop_Data *pd)
{
   _add_1check_color_item(_("strikethrough color"), pd->box_lines_frame, &pd->check_s_color, &pd->s_color);
   evas_object_smart_callback_add(pd->check_s_color, "changed", _on_check_s_color_change, pd);
   evas_object_event_callback_add(pd->s_color, EVAS_CALLBACK_MOUSE_DOWN,
                                  _on_s_color_clicked, pd);

   _add_1check_2button_item(_("underline"), pd->box_lines_frame, &pd->check_unline, &pd->segment_ctrl);
   evas_object_smart_callback_add(pd->check_unline, "changed", _on_check_unline_change, pd);
   evas_object_smart_callback_add(pd->segment_ctrl, "changed", _on_segment_ctrl_change, pd);

   _prop_text_2color_add(_("underline color"), pd->box_lines_frame, &pd->outer_color_singl, &pd->outer_color_double);
   evas_object_event_callback_add(pd->outer_color_singl, EVAS_CALLBACK_MOUSE_DOWN,
                                  _on_outer_color_singl_clicked, pd);
   evas_object_event_callback_add(pd->outer_color_double, EVAS_CALLBACK_MOUSE_DOWN,
                                  _on_outer_color_double_clicked, pd);
}

static Evas_Object *
_add_lines_frame(Style_Prop_Data *pd, Evas_Object *parent)
{
   Evas_Object *frame_lines;

   FRAME_ADD(parent, frame_lines, true, _("Lines"))

   BOX_ADD(frame_lines, pd->box_lines_frame, EINA_FALSE, EINA_FALSE);
   elm_box_padding_set(pd->box_lines_frame, 6, 0);
   elm_object_content_set(frame_lines, pd->box_lines_frame);

   return frame_lines;
}

static void
_position_text_option_update(Style_Prop_Data *pd, const char *value)
{
   Eina_Bool pass, bg;
   int r, g, b, a;
   unsigned int i;
   if (value)
     {
        Eina_Tmpstr *align = _tag_value_get(value, "align");
        if (!align) align = eina_tmpstr_add("auto");
        Eina_Tmpstr *lmargin = _tag_value_get(value, "left_margin");
        if (!lmargin) lmargin = eina_tmpstr_add("0");
        Eina_Tmpstr *valign = _tag_value_get(value, "valign");
        if (!valign) valign = eina_tmpstr_add("baseline");
        Eina_Tmpstr *rmargin = _tag_value_get(value, "right_margin");
        if (!rmargin) rmargin = eina_tmpstr_add("0");
        Eina_Tmpstr *vwrap = _tag_value_get(value, "wrap");
        if (!vwrap) vwrap = eina_tmpstr_add("none");

        ewe_combobox_text_set(pd->hor_align, align);
        for (i = 0; font_horizontal_align[i] != NULL; i++)
          ewe_combobox_item_add(pd->hor_align, font_horizontal_align[i]);
        elm_spinner_value_set(pd->left_margin, atof(lmargin));

        ewe_combobox_text_set(pd->vert_align, valign);
        for (i = 0; font_horizontal_valign[i] != NULL; i++)
          ewe_combobox_item_add(pd->vert_align, font_horizontal_valign[i]);
        elm_spinner_value_set(pd->right_margin, atof(rmargin));

        ewe_combobox_text_set(pd->wrap, vwrap);
        for (i = 0; text_wrap[i] != NULL; i++)
          ewe_combobox_item_add(pd->wrap, text_wrap[i]);

        eina_tmpstr_del(align);
        eina_tmpstr_del(lmargin);
        eina_tmpstr_del(valign);
        eina_tmpstr_del(vwrap);
        eina_tmpstr_del(rmargin);
     }

   if (value)
     {
        Eina_Tmpstr *tabstops = _tag_value_get(value, "tabstops");
        if (!tabstops) tabstops = eina_tmpstr_add("0");
        Eina_Tmpstr *linesize = _tag_value_get(value, "linesize");
        if (!linesize) linesize = eina_tmpstr_add("0");
        Eina_Tmpstr *linerelsize = _tag_value_get(value, "linerelsize");
        if (!linerelsize) linerelsize = eina_tmpstr_add("0");
        Eina_Tmpstr *password = _tag_value_get(value, "password");
        if ((!password) || (!strcmp(password, "off"))) pass = EINA_FALSE;
        else pass = EINA_TRUE;

        Eina_Tmpstr *ellipsis = _tag_value_get(value, "ellipsis");
        if (!ellipsis) ellipsis = eina_tmpstr_add("-1.0");

        Eina_Tmpstr *check_ellipsis = _tag_value_get(value, "check_ellipsis");
        if ((!check_ellipsis) || (!strcmp(check_ellipsis, "off")))
          {
             elm_object_disabled_set(pd->replace_ellisis, true);
             ellipsis = eina_tmpstr_add("-1.0");
          }
        else
          elm_object_disabled_set(pd->replace_ellisis, false);

        Eina_Tmpstr *bground = _tag_value_get(value, "backing");
        if ((!bground) || (!strcmp(bground, "off"))) bg = EINA_FALSE;
        else bg = EINA_TRUE;
        Eina_Tmpstr *bcolor = _tag_value_get(value, "backing_color");
        if (!bcolor)
          {
             bcolor = eina_tmpstr_add(WHITE_COLOR);
             _tag_parse(pd, WHITE_COLOR, "backing_color");
          }

        elm_spinner_value_set(pd->tab_stops, atof(tabstops));
        elm_spinner_value_set(pd->line_size, atof(linesize));
        elm_spinner_value_set(pd->relative_size, atof(linerelsize));
        elm_check_state_set(pd->check_psw, pass);
        elm_check_state_set(pd->check_color, bg);
        _entry_repch_update(pd, pass);
        if (!_hex_to_rgb(bcolor, &r, &g, &b, &a))
          ERR("This error should not happen in style editor... Contact devs please!");
        evas_object_color_set(pd->bg_color, r*a/255, g*a/255, b*a/255, a);
        elm_spinner_value_set(pd->replace_ellisis, atof(ellipsis) * 100);
        eina_tmpstr_del(tabstops);
        eina_tmpstr_del(linesize);
        eina_tmpstr_del(linerelsize);
        eina_tmpstr_del(password);
        eina_tmpstr_del(ellipsis);
        eina_tmpstr_del(bground);
        eina_tmpstr_del(bcolor);
     }
}

static Eina_Bool
_check_value(const char *list[], const char *value)
{
   Eina_Bool exist = false;
   int i = 0;

   for (i = 0; list[i] != NULL; i++)
     {
        if (!strcmp(value, list[i]))
          {
             exist = true;
             break;
          }
     }

   return exist;
}

static void
_glow_shadow_prop_update(Style_Prop_Data *pd, const char *value)
{
   int r, g, b, a;
   unsigned int i = 0;
   char *style_copy = NULL;
   char *token;
   int count = 0, direction = DEFAULT_DIRECTION;

   if (value)
     {
        elm_box_clear(pd->box_glow_shadow);
        Eina_Tmpstr *style = _tag_value_get(value, "style");
        if (!style) style = eina_tmpstr_add("none");
        Eina_Tmpstr *inner = _tag_value_get(value, "glow_color");
        if (!inner)
          {
             inner = eina_tmpstr_add(WHITE_COLOR);
             _tag_parse(pd, WHITE_COLOR, "glow_color");
          }
        Eina_Tmpstr *outer = _tag_value_get(value, "glow2_color");
        if (!outer)
          {
              outer = eina_tmpstr_add(WHITE_COLOR);
             _tag_parse(pd, WHITE_COLOR, "glow2_color");
          }
        Eina_Tmpstr *shadow = _tag_value_get(value, "shadow_color");
        if (!shadow)
          {
             shadow = eina_tmpstr_add(WHITE_COLOR);
             _tag_parse(pd, WHITE_COLOR, "shadow_color");
          }

        style_copy = mem_malloc(strlen(style) + 1);
        strcpy(style_copy, style);
        token = strtok(style_copy, ",");
        TODO("replace with eina_str_split_full")
        while (token)
          {
             if (count == 0)
               {
                  eina_tmpstr_del(style);
                  style = eina_tmpstr_add(token);
                  count++;
               }
             else
               {
                  for (i = 0; direction_list[i] != NULL; i++)
                    {
                       if (!strcmp(direction_list[i], token)) direction = i;
                    }
               }
             token = strtok(0, " ");
          }
        free(style_copy);

        if (!_check_value(font_glow_list, style))
          {
             eina_tmpstr_del(style);
             style = eina_tmpstr_add("none");
             ERR("Wrong style name. Find who did this loaded Edj-Theme and slap his face.");
          }

        if (strstr(style, "shadow"))
          {
             _add_shadow_part(pd);
             if (!_hex_to_rgb(shadow, &r, &g, &b, &a))
               ERR("This error should not happen in style editor... Contact devs please!");
             evas_object_color_set(pd->shadow_color, r*a/255, g*a/255, b*a/255, a);

             ewe_combobox_select_item_set(pd->direction, direction);
          }
        else
          {
             _add_inner_outer_part(pd);
             if (!_hex_to_rgb(inner, &r, &g, &b, &a))
               ERR("This error should not happen in style editor... Contact devs please!");
             evas_object_color_set(pd->inner_gl_color, r*a/255, g*a/255, b*a/255, a);
             if (!_hex_to_rgb(outer, &r, &g, &b, &a))
               ERR("This error should not happen in style editor... Contact devs please!");
             evas_object_color_set(pd->outer_gl_color, r*a/255, g*a/255, b*a/255, a);
          }

        ewe_combobox_text_set(pd->style, style);
        for (i = 0; font_glow_list[i] != NULL; i++)
          ewe_combobox_item_add(pd->style, font_glow_list[i]);

        eina_tmpstr_del(style);
        eina_tmpstr_del(inner);
        eina_tmpstr_del(outer);
        eina_tmpstr_del(shadow);
     }
}

static void
_lines_prop_update(Style_Prop_Data *pd, const char *value)
{
   Elm_Object_Item *sc_item;
   Eina_Bool strikethr, underl;
   int r, g, b, a;
   unsigned int i = 0;

   assert(pd != NULL);
   if (value)
     {
        Eina_Tmpstr *strikethrough = _tag_value_get(value, "strikethrough");
        if ((!strikethrough) || (!strcmp(strikethrough, "off"))) strikethr = EINA_FALSE;
        else strikethr = EINA_TRUE;
        Eina_Tmpstr *strikethru_color = _tag_value_get(value, "strikethrough_color");
        if (!strikethru_color) strikethru_color = eina_tmpstr_add(WHITE_COLOR);
        Eina_Tmpstr *seg_item = NULL;
        Eina_Tmpstr *underline = _tag_value_get(value, "underline");
        if ((!underline) || (!strcmp(underline, "off"))) underl = EINA_FALSE;
        else
          {
             underl = EINA_TRUE;
             if (!strcmp(underline, "double")) seg_item = eina_tmpstr_add("double");
             else seg_item = eina_tmpstr_add("single");
             for (i = 0; underl_styles[i] != NULL; i++)
               {
                  if (!strcmp(elm_segment_control_item_label_get(pd->segment_ctrl, i), seg_item))
                    {
                       sc_item = elm_segment_control_item_get(pd->segment_ctrl, i);
                       elm_segment_control_item_selected_set(sc_item, true);
                    }
               }
          }
        Eina_Tmpstr *underl_color = _tag_value_get(value, "underline_color");
        if (!underl_color) underl_color = eina_tmpstr_add(WHITE_COLOR);
        Eina_Tmpstr *underl2_color = _tag_value_get(value, "underline2_color");
        if (!underl2_color) underl2_color = eina_tmpstr_add(WHITE_COLOR);

        elm_check_state_set(pd->check_s_color, strikethr);
        elm_check_state_set(pd->check_unline, underl);
        if (!_hex_to_rgb(strikethru_color, &r, &g, &b, &a))
          ERR("This error should not happen in style editor... Contact devs please!");
        evas_object_color_set(pd->s_color, r*a/255, g*a/255, b*a/255, a);
        if (!_hex_to_rgb(underl_color, &r, &g, &b, &a))
          ERR("This error should not happen in style editor... Contact devs please!");
        evas_object_color_set(pd->outer_color_singl, r*a/255, g*a/255, b*a/255, a);
        if (!_hex_to_rgb(underl2_color, &r, &g, &b, &a))
          ERR("This error should not happen in style editor... Contact devs please!");
        evas_object_color_set(pd->outer_color_double, r*a/255, g*a/255, b*a/255, a);

        eina_tmpstr_del(seg_item);
        eina_tmpstr_del(strikethrough);
        eina_tmpstr_del(strikethru_color);
        eina_tmpstr_del(underline);
        eina_tmpstr_del(underl_color);
        eina_tmpstr_del(underl2_color);
     }
}

static void
_text_prop_update(Style_Prop_Data *pd, const char *value)
{
   int i;
   int r, g, b, a;

   if (value)
     {
        Eina_Tmpstr *font = _tag_value_get(value, "font");
        if (!font) font = eina_tmpstr_add("");
        Eina_Tmpstr *spin_val = _tag_value_get(value, "font_size");
        if (!spin_val) spin_val = eina_tmpstr_add("");
        Eina_Tmpstr *weight = _tag_value_get(value, "font_weight");
        if (!weight) weight = eina_tmpstr_add("Normal");
        Eina_Tmpstr *width = _tag_value_get(value, "font_width");
        if (!width) width = eina_tmpstr_add("Normal");

        Eina_Tmpstr *color = _tag_value_get(value, "color");

        elm_entry_entry_set(pd->font_name, font);
        elm_spinner_value_set(pd->font_size, atof(spin_val));

        ewe_combobox_text_set(pd->font_style, width);
        for (i = 0; font_width_list[i] != NULL; i++)
          {
             ewe_combobox_item_add(pd->font_style, font_width_list[i]);
          }
        ewe_combobox_text_set(pd->font_weight, weight);
        for (i = 0; font_weight_list[i] != NULL; i++)
          {
             ewe_combobox_item_add(pd->font_weight, font_weight_list[i]);
          }

        if (!_hex_to_rgb(color, &r, &g, &b, &a))
          {
             ERR("Can't convert color value");
             abort();
          }
        evas_object_color_set(pd->font_color, r*a/255, g*a/255, b*a/255, a);

        eina_tmpstr_del(font);
        eina_tmpstr_del(spin_val);
        eina_tmpstr_del(color);
        eina_tmpstr_del(weight);
        eina_tmpstr_del(width);
     }
}

static void
_clear_prop_style(Style_Prop_Data *pd)
{
   elm_box_clear(pd->box_frame_text);
   elm_box_clear(pd->box_frame_position);
   elm_box_clear(pd->box_format_frame);
   elm_box_clear(pd->box_glow_shadow);
   elm_box_clear(pd->box_lines_frame);
}

static void
_on_style_selected(void *data ,
                   Evas_Object *obj __UNUSED__,
                   void *event_info)
{
   Style_Data *current_style = (Style_Data *)event_info;
   Evas_Object *property = (Evas_Object *)data;
   STYLE_PROP_DATA_GET()

   _clear_prop_style(pd);
   if (!current_style) return;

   CURRENT = *current_style;
   if (!CURRENT.is_parent_item)
     {
        _add_text_part(pd);
        _text_prop_update(pd, eina_strbuf_string_get(CURRENT.style));

        _add_lines_part(pd);
        _lines_prop_update(pd, eina_strbuf_string_get(CURRENT.style));

        _add_position_part(pd);
        _add_text_format_part(pd);
        _position_text_option_update(pd, eina_strbuf_string_get(CURRENT.style));

        _glow_shadow_prop_update(pd, eina_strbuf_string_get(CURRENT.style));
     }

   free(current_style);
   eina_strbuf_free(CURRENT.style);
}

static void
_on_property_style_del(void * data,
                       Evas *e __UNUSED__,
                       Evas_Object *obj,
                       void *event_info __UNUSED__)
{
   Style_Prop_Data *pd = (Style_Prop_Data *)data;

   evas_object_data_del(obj, STYLE_PROP_DATA);
   evas_object_event_callback_del(obj, EVAS_CALLBACK_DEL, _on_property_style_del);
   free(pd);
}

Evas_Object *
ui_property_style_add(Evas_Object *parent)
{
   Style_Prop_Data *pd;
   Evas_Object *scroller, *main_box;

   assert(parent != NULL);

   pd = mem_calloc(1, sizeof(Style_Prop_Data));

   SCROLLER_ADD(parent, scroller);

   BOX_ADD(scroller, main_box, EINA_FALSE, EINA_FALSE);
   elm_box_align_set(main_box, 0.5, 0.0);
   elm_object_content_set(scroller, main_box);

   elm_scroller_policy_set(scroller, ELM_SCROLLER_POLICY_AUTO, ELM_SCROLLER_POLICY_AUTO);

   elm_box_pack_end(main_box, _add_text_frame(pd, main_box));
   elm_box_pack_end(main_box, _add_position_frame(pd, main_box));
   elm_box_pack_end(main_box, _add_text_format_frame(pd, main_box));
   elm_box_pack_end(main_box, _add_glow_shadow_frame(pd, main_box));
   elm_box_pack_end(main_box, _add_lines_frame(pd, main_box));

   evas_object_data_set(main_box, STYLE_PROP_DATA, pd);

   evas_object_smart_callback_add(ap.win, SIGNAL_STYLE_SELECTED, _on_style_selected, main_box);
   evas_object_event_callback_add(main_box, EVAS_CALLBACK_DEL, _on_property_style_del, pd);

   return scroller;
}

#undef FONT_DEFAULT
#undef TAB_UPDATE
#undef STYLE_PROP_DATA_GET
#undef ADD_1SWALLOW_ITEM
#undef ADD_COLOR_ELEMENT
#undef SPINNER_VALUE
#undef COMBOBOX_VALUE
#undef COMBOBOX_NONE_VALUE
#undef PERCENT_SPINNER_VALUE
#undef CHECK_VALUE
#undef SEGMENT_VALUE
#undef CALLBACKS_COLOR_ADD
#undef DIRECT_ADD

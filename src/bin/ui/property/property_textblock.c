#include "property.h"
#include "property_private.h"
#include "project_manager2.h"

#define FONT_DEFAULT "DEFAULT='align=middle font=Sans font_size=24 color=#000000 "
#define DIRECTION_NUM 40
#define WHITE_COLOR "#FFF"

struct _Property_Textblock_Data {
   Eina_Stringshare *font;
   int font_size;
   int font_style_weight;
   int font_style_width;
   struct {
      int r, g, b, a;
   } color, bg_color, outer_color, inner_color, shadow_color;
   int font_align_hor;
   int font_valign_hor;
   int left_margin, right_margin;
   int wrap;
   int linerelsize, linesize, tabstops;
   Eina_Bool pass, bg_check, ellipsis_check;
   int ellipsis_value;
   int glow_style;
   int direction;
   Eina_Bool strikethrough_check;
   struct {
      int r, g, b, a;
   } underone_color, undertwo_color, strikethrough_color, dash_color;
   int underline;
   int dash_width, dash_gap;

   Style_Data current_style;
   Eina_Bool selected;
   Property_Attribute items[PROPERTY_TEXTBLOCK_ITEM_LAST];
};
typedef struct _Property_Textblock_Data Property_Textblock_Data;
static Property_Textblock_Data textblock_pd;
#define tpd textblock_pd

struct _Property_Textblock_Update_Info {
   Property_Attribute *pa;
   Property_Action *action;
};
typedef struct _Property_Textblock_Update_Info Property_Textblock_Update_Info;

/* since font name should be without any spaces here we go:
   let's block those spaces */
static Elm_Entry_Filter_Accept_Set accept_set = {
     .accepted = NULL,
     .rejected = " "
};

/* array to find item by Attribute */
static Property_Textblock_Update_Info attribute_map[ATTRIBUTE_TEXTBLOCK_ITEM_LAST];

static const char *font_weight_list[] = { "Normal",
                                          "Thin",
                                          "Ultralight",
                                          "Light",
                                          "Book",
                                          "Medium",
                                          "Semibold",
                                          "Bold",
                                          "Ultrabold",
                                          "Black",
                                          "Extrablack",
                                          NULL};

static const char *font_width_list[] = { "Normal",
                                         "Ultracondensed",
                                         "Extracondensed",
                                         "Condensed",
                                         "Semicondensed",
                                         "Semiexpanded",
                                         "Expanded",
                                         "Extraexpanded",
                                         "Ultraexpanded",
                                         NULL};

static const char *font_horizontal_align[] = { "auto",
                                               "center",
                                               "middle",
                                               "left",
                                               "right",
                                               NULL};

static const char *font_horizontal_valign[] = { "top",
                                                "bottom",
                                                "middle",
                                                "center",
                                                "baseline",
                                                "base",
                                                NULL};

static const char *text_wrap[] = { "none",
                                   "word",
                                   "char",
                                   "mixed",
                                   "hyphenation",
                                   NULL};

static const char *font_glow_list[] = { "none",
                                        "plain",
                                        "shadow",
                                        "outline",
                                        "soft_outline",
                                        "outline_shadow",
                                        "outline_soft_shadow",
                                        "glow",
                                        "far_shadow",
                                        "soft_shadow",
                                        "far_soft_shadow",
                                        NULL};

static const char *direction_list[] = { "bottom_left",
                                        "bottom",
                                        "bottom_right",
                                        "left",
                                        "right",
                                        "top_left",
                                        "top",
                                        "top_right",
                                        NULL};

#define SINGLE_UNDERLINE_NUMBER 1
#define DOUBLE_UNDERLINE_NUMBER 2
#define DASHED_UNDERLINE_NUMBER 3
static const char *underl_styles[] = { "off",        /* 0 */
                                       "single",     /* 1 */
                                       "double",     /* 2 */
                                       "dashed",     /* 3 */
                                       NULL};

/************************************************************************/
/*************        LOCAL FUNCTIONS TO SPLIT STYLE         ************/
/************************************************************************/
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

static int
_combobox_get_num(Eina_Tmpstr *tmp, const char **array)
{
   int i = 0;

   assert(tmp != NULL);
   assert(array != NULL);

   while (array[i] != NULL)
     {
        if (!strcmp(tmp, array[i])) return i;
        ++i;
     }
   return 0; /* NONE */
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
   int length = (int)strlen_safe(hex);
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

static Eina_Tmpstr*
_tag_value_get(const char* text_style, char* a_tag)
{
   assert(text_style != NULL);
   assert(a_tag != NULL);

   char *tag_list_copy = mem_malloc(strlen_safe(text_style) + 1);
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

                  if (equals_sign[strlen_safe(equals_sign) - 1] != '\'')
                    result = eina_tmpstr_add(equals_sign);
                  else
                    result = eina_tmpstr_add_length(equals_sign, strlen_safe(equals_sign) - 1);
                  if (!strstr(FONT_DEFAULT, a_tag)) break;
               }
          }
        token = strtok(0, " ");
     }
   free(tag_list_copy);

   return result;
}

static void
_entry_repch_update(Eina_Bool password)
{
   if (password)
     evas_object_textblock_replace_char_set(tpd.current_style.textblock_style, "*");
   else
     evas_object_textblock_replace_char_set(tpd.current_style.textblock_style, NULL);
}

inline static void
_print_string(Eina_Strbuf **tag, const char *param, const char *text)
{
   eina_strbuf_append(*tag, param);
   eina_strbuf_append(*tag, text);
}

inline static void
_print_data_string(Eina_Strbuf **tag, const char *param, int value)
{
   Eina_Stringshare *str_tmp;

   eina_strbuf_append(*tag, param);
   str_tmp = eina_stringshare_printf("%d", value);
   eina_strbuf_append(*tag, str_tmp);
   eina_stringshare_del(str_tmp);
}

inline static void
_print_color_string(Eina_Strbuf **tag, const char *param, int r, int g, int b, int a)
{
   Eina_Stringshare *str_tmp;

   eina_strbuf_append(*tag, param);
   str_tmp = eina_stringshare_printf("#%02x%02x%02x%02x", r, g, b, a);
   eina_strbuf_append(*tag, str_tmp);
   eina_stringshare_del(str_tmp);
}

static void
_update_style()
{
   Eina_Strbuf *tag = eina_strbuf_new();
   Eina_Stringshare *str_tmp;

   TODO("Implement or think about next params in a comment below")
   /* "font_fallback" "font_source" "font_style" "lang" "outline_color"
      "linegap" "linerelgap" "item" "linefill" */
   if (strcmp(tpd.current_style.st_tag, "DEFAULT"))
     eina_strbuf_append(tag, " + ");

   _print_string(&tag, " font=", tpd.font);

   _print_data_string(&tag, " font_size=", tpd.font_size);
   _print_string(&tag, " font_weight=", font_weight_list[tpd.font_style_weight]);
   _print_string(&tag, " font_width=", font_width_list[tpd.font_style_width]);

   _print_color_string(&tag, " color=", tpd.color.r, tpd.color.g, tpd.color.b, tpd.color.a);
   _print_color_string(&tag, " underline_color=", tpd.underone_color.r, tpd.underone_color.g, tpd.underone_color.b, tpd.underone_color.a);
   _print_color_string(&tag, " underline2_color=", tpd.undertwo_color.r, tpd.undertwo_color.g, tpd.undertwo_color.b, tpd.undertwo_color.a);
   _print_color_string(&tag, " underline_dash_color=", tpd.dash_color.r, tpd.dash_color.g, tpd.dash_color.b, tpd.dash_color.a);
   _print_color_string(&tag, " shadow_color=", tpd.shadow_color.r, tpd.shadow_color.g, tpd.shadow_color.b, tpd.shadow_color.a);
   _print_color_string(&tag, " glow_color=", tpd.inner_color.r, tpd.inner_color.g, tpd.inner_color.b, tpd.inner_color.a);
   _print_color_string(&tag, " glow2_color=", tpd.outer_color.r, tpd.outer_color.g, tpd.outer_color.b, tpd.outer_color.a);
   _print_color_string(&tag, " backing_color=", tpd.bg_color.r, tpd.bg_color.g, tpd.bg_color.b, tpd.bg_color.a);
   _print_color_string(&tag, " strikethrough_color=", tpd.strikethrough_color.r, tpd.strikethrough_color.g, tpd.strikethrough_color.b, tpd.strikethrough_color.a);

   _print_string(&tag, " align=", font_horizontal_align[tpd.font_align_hor]);
   _print_string(&tag, " valign=", font_horizontal_valign[tpd.font_valign_hor]);
   _print_string(&tag, " wrap=", text_wrap[tpd.wrap]);

   _print_data_string(&tag, " left_margin=", tpd.left_margin);
   _print_data_string(&tag, " right_margin=", tpd.right_margin);

   _print_string(&tag, " underline=", underl_styles[tpd.underline]);
   _print_string(&tag, " strikethrough=", (tpd.strikethrough_check) ? "on " : "off ");
   _print_string(&tag, " backing=", (tpd.bg_check) ? "on " : "off ");

   /* unique case with style and direction for glow shadow */
   eina_strbuf_append(tag, " style=");
   eina_strbuf_append(tag, font_glow_list[tpd.glow_style]);
   if (strstr(font_glow_list[tpd.glow_style], "shadow"))
     {
        eina_strbuf_append(tag, ",");
        eina_strbuf_append(tag, direction_list[tpd.direction]);
     }

   _print_data_string(&tag, " tabstops=", tpd.tabstops);
   _print_data_string(&tag, " linesize=", tpd.linesize);
   _print_data_string(&tag, " linerelsize=", tpd.linerelsize);
   _print_data_string(&tag, " underline_dash_width=", tpd.dash_width);
   _print_data_string(&tag, " underline_dash_gap=", tpd.dash_gap);
   _print_string(&tag, " password=", (tpd.pass) ? "on " : "off ");
   _entry_repch_update(tpd.pass);

   /* unique case with ellipsis */
   if (tpd.ellipsis_check)
     {
        eina_strbuf_append(tag, " ellipsis=");
        str_tmp = eina_stringshare_printf("%f", (double)tpd.ellipsis_value / 100);
        eina_strbuf_append(tag, str_tmp);
        eina_stringshare_del(str_tmp);
     }

   CRIT_ON_FAIL(editor_style_tag_value_set(ap.project->global_object, tpd.current_style.st_name,
                                           tpd.current_style.st_tag, eina_strbuf_string_get(tag)));
   eina_stringshare_del(tpd.current_style.stvalue);
   tpd.current_style.stvalue = eina_stringshare_add(eina_strbuf_string_get(tag));
   eina_strbuf_free(tag);
}

static void
_style_edit_update()
{
   Evas_Textblock_Style *ts = NULL;
   Eina_Strbuf *style = eina_strbuf_new();

   _update_style();

   eina_strbuf_append(style, FONT_DEFAULT);
   eina_strbuf_append(style, tpd.current_style.stvalue);
   eina_strbuf_append(style, "'");
   ts = evas_textblock_style_new();
   evas_textblock_style_set(ts, eina_strbuf_string_get(style));
   evas_object_textblock_style_set(tpd.current_style.textblock_style, ts);
   evas_object_size_hint_max_set(tpd.current_style.textblock_style, EVAS_HINT_FILL,
                                 EVAS_HINT_FILL);
   eina_strbuf_free(style);
   evas_textblock_style_free(ts);
}

/************************************************************************/
/************************************************************************/
/************************************************************************/

/* local callbacks */
static void
_fill_combobox_with_enum(Evas_Object *control, const char **array)
{
   unsigned int i = 0;
   Combobox_Item *combobox_item;
   Elm_Genlist_Item_Class *itc;

   assert(control != NULL);
   assert(array != NULL);

   itc = evas_object_data_get(control, "COMMON_ITC");

   while (array[i] != NULL)
     {
        combobox_item = mem_malloc(sizeof(Combobox_Item));
        combobox_item->index = i;
        combobox_item->data = eina_stringshare_add(array[i]);
        elm_genlist_item_append(control, itc,
                                combobox_item, NULL,
                                ELM_GENLIST_ITEM_NONE, NULL, NULL);
        ++i;
     }
}


static Eina_List *
_subitems_get(Property_Attribute *pa)
{
   Eina_List *items = NULL;

   assert(pa != NULL);
#define APPEND(TYPE) items = eina_list_append(items, &textblock_pd.items[TYPE]);
   switch (pa->type.textblock_item)
     {
      case PROPERTY_TEXTBLOCK_ITEM_TEXT_TITLE:
         APPEND(PROPERTY_TEXTBLOCK_ITEM_TEXT_FONT_NAME);
         APPEND(PROPERTY_TEXTBLOCK_ITEM_TEXT_FONT_STYLE_WEIGHT);
         APPEND(PROPERTY_TEXTBLOCK_ITEM_TEXT_FONT_STYLE_WIDTH);
         APPEND(PROPERTY_TEXTBLOCK_ITEM_TEXT_COLOR);
         APPEND(PROPERTY_TEXTBLOCK_ITEM_TEXT_SIZE);
         break;
      case PROPERTY_TEXTBLOCK_ITEM_POSITION_TITLE:
         APPEND(PROPERTY_TEXTBLOCK_ITEM_POSITION_ALIGN_HOR);
         APPEND(PROPERTY_TEXTBLOCK_ITEM_POSITION_ALIGN_VER);
         APPEND(PROPERTY_TEXTBLOCK_ITEM_POSITION_MARGIN);
         APPEND(PROPERTY_TEXTBLOCK_ITEM_POSITION_WRAP);
         break;
      case PROPERTY_TEXTBLOCK_ITEM_FORMAT_TITLE:
         APPEND(PROPERTY_TEXTBLOCK_ITEM_FORMAT_TABSTOPS);
         APPEND(PROPERTY_TEXTBLOCK_ITEM_FORMAT_LINE_SIZE);
         APPEND(PROPERTY_TEXTBLOCK_ITEM_FORMAT_BG_COLOR);
         APPEND(PROPERTY_TEXTBLOCK_ITEM_FORMAT_PASSWORD);
         APPEND(PROPERTY_TEXTBLOCK_ITEM_FORMAT_ELLIPSIS);
         APPEND(PROPERTY_TEXTBLOCK_ITEM_FORMAT_LINE_RELATED_SIZE);
         break;
      case PROPERTY_TEXTBLOCK_ITEM_GLOW_SHADOW_TITLE:
         APPEND(PROPERTY_TEXTBLOCK_ITEM_GLOW_SHADOW_STYLE);
         APPEND(PROPERTY_TEXTBLOCK_ITEM_GLOW_SHADOW_COLOR);
         APPEND(PROPERTY_TEXTBLOCK_ITEM_GLOW_SHADOW_DIRECTION);
         APPEND(PROPERTY_TEXTBLOCK_ITEM_GLOW_SHADOW_OUTER_COLOR);
         APPEND(PROPERTY_TEXTBLOCK_ITEM_GLOW_SHADOW_INNER_COLOR);
         break;
      case PROPERTY_TEXTBLOCK_ITEM_LINES_TITLE:
         APPEND(PROPERTY_TEXTBLOCK_ITEM_LINES_STRIKETHROUGH);
         APPEND(PROPERTY_TEXTBLOCK_ITEM_LINES_UNDERLINE);
         APPEND(PROPERTY_TEXTBLOCK_ITEM_LINES_UNDERLINE_COLOR_ONE);
         APPEND(PROPERTY_TEXTBLOCK_ITEM_LINES_UNDERLINE_COLOR_TWO);
         APPEND(PROPERTY_TEXTBLOCK_ITEM_LINES_UNDERLINE_DASH_COLOR);
         APPEND(PROPERTY_TEXTBLOCK_ITEM_LINES_UNDERLINE_DASH);
         break;
      default:
         CRIT("items callback not found for %s", pa->name);
         abort();
     }

   return items;
#undef APPEND
}

static void
_change_cb(Property_Attribute *pa, Property_Action *action)
{
   Eina_Stringshare *str_val1 = NULL;
   Combobox_Item *cb_item = NULL;
   double double_val1 = 0.0;
   Eina_Bool bool_val1 = false;
   int r = -1, g = -1, b = -1, a = -1;

   assert(pa != NULL);
   assert(action != NULL);
   assert(action->control != NULL);

   PROPERTY_DATA_GET(action->control);

   switch (action->control_type)
     {
      case PROPERTY_CONTROL_CHECK:
         bool_val1 = elm_check_state_get(action->control);
         break;
      case PROPERTY_CONTROL_SPINNER:
         double_val1 = elm_spinner_value_get(action->control);
         break;
      case PROPERTY_CONTROL_ENTRY:
         str_val1 = property_entry_get(action->control);
         break;
      case PROPERTY_CONTROL_COMBOBOX:
         cb_item = evas_object_data_get(action->control, "CURRENT_DATA");
         if (!cb_item) return;
         break;
      case PROPERTY_CONTROL_COLOR:
         property_color_control_color_get(action->control, &r, &g, &b, &a);
         break;
      default:
         break;
     }

   switch (action->type.attribute_textblock)
     {
      case ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_FONT_NAME:
         TODO("Refactor this");
         Font_Change font_change;
         font_change.old_value = eina_stringshare_add(tpd.font);
         font_change.value = eina_stringshare_add(str_val1);
         font_change.style_name = eina_stringshare_add(tpd.current_style.st_name);
         font_change.tag_name = eina_stringshare_add(tpd.current_style.st_tag);

         evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_STYLE_TAG_CHANGED, &font_change);

         if (tpd.font)
           eina_stringshare_del(tpd.font);
         tpd.font = eina_stringshare_add(str_val1);
         _style_edit_update();
         CRIT_ON_FAIL(editor_save(ap.project->global_object));
         ap.project->changed = true;

         eina_stringshare_del(font_change.old_value);
         eina_stringshare_del(font_change.value);
         eina_stringshare_del(font_change.tag_name);
         eina_stringshare_del(font_change.style_name);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_FONT_STYLE_WEIGHT:
         assert(cb_item != NULL);
         tpd.font_style_weight = cb_item->index;
         _style_edit_update();
         CRIT_ON_FAIL(editor_save(ap.project->global_object));
         ap.project->changed = true;
         elm_object_text_set(action->control, font_weight_list[tpd.font_style_weight]);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_FONT_STYLE_WIDTH:
         assert(cb_item != NULL);
         tpd.font_style_width = cb_item->index;
         _style_edit_update();
         CRIT_ON_FAIL(editor_save(ap.project->global_object));
         ap.project->changed = true;
         elm_object_text_set(action->control, font_width_list[tpd.font_style_width]);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_COLOR:
         tpd.color.r = r;
         tpd.color.g = g;
         tpd.color.b = b;
         tpd.color.a = a;
         _style_edit_update();
         CRIT_ON_FAIL(editor_save(ap.project->global_object));
         ap.project->changed = true;
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_SIZE:
         tpd.font_size = double_val1;
         _style_edit_update();
         CRIT_ON_FAIL(editor_save(ap.project->global_object));
         ap.project->changed = true;
         break;

      case ATTRIBUTE_TEXTBLOCK_ITEM_POSITION_ALIGN_HOR:
         assert(cb_item != NULL);
         tpd.font_align_hor = cb_item->index;
         _style_edit_update();
         CRIT_ON_FAIL(editor_save(ap.project->global_object));
         ap.project->changed = true;
         elm_object_text_set(action->control, font_horizontal_align[tpd.font_align_hor]);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_POSITION_ALIGN_VER:
         assert(cb_item != NULL);
         tpd.font_valign_hor = cb_item->index;
         _style_edit_update();
         CRIT_ON_FAIL(editor_save(ap.project->global_object));
         ap.project->changed = true;
         elm_object_text_set(action->control, font_horizontal_valign[tpd.font_valign_hor]);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_POSITION_MARGIN_LEFT:
         tpd.left_margin = double_val1;
         _style_edit_update();
         CRIT_ON_FAIL(editor_save(ap.project->global_object));
         ap.project->changed = true;
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_POSITION_MARGIN_RIGHT:
         tpd.right_margin = double_val1;
         _style_edit_update();
         CRIT_ON_FAIL(editor_save(ap.project->global_object));
         ap.project->changed = true;
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_POSITION_WRAP:
         assert(cb_item != NULL);
         tpd.wrap = cb_item->index;
         _style_edit_update();
         CRIT_ON_FAIL(editor_save(ap.project->global_object));
         ap.project->changed = true;
         elm_object_text_set(action->control, text_wrap[tpd.wrap]);
         break;

      case ATTRIBUTE_TEXTBLOCK_ITEM_FORMAT_TABSTOPS:
         tpd.tabstops = double_val1;
         _style_edit_update();
         CRIT_ON_FAIL(editor_save(ap.project->global_object));
         ap.project->changed = true;
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_FORMAT_LINE_SIZE:
         tpd.linesize = double_val1;
         _style_edit_update();
         CRIT_ON_FAIL(editor_save(ap.project->global_object));
         ap.project->changed = true;
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_FORMAT_BG_COLOR_CHECK:
         tpd.bg_check = bool_val1;
         _style_edit_update();
         CRIT_ON_FAIL(editor_save(ap.project->global_object));
         ap.project->changed = true;
         elm_object_disabled_set(pa->action2.control, !tpd.bg_check);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_FORMAT_BG_COLOR_COLOR:
         tpd.bg_color.r = r;
         tpd.bg_color.g = g;
         tpd.bg_color.b = b;
         tpd.bg_color.a = a;
         _style_edit_update();
         CRIT_ON_FAIL(editor_save(ap.project->global_object));
         ap.project->changed = true;
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_FORMAT_PASSWORD:
         tpd.pass = bool_val1;
         _style_edit_update();
         CRIT_ON_FAIL(editor_save(ap.project->global_object));
         ap.project->changed = true;
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_FORMAT_ELLIPSIS_CHECK:
         tpd.ellipsis_check = bool_val1;
         if (!bool_val1)
           tpd.ellipsis_value = -100;
         else
           tpd.ellipsis_value = 0;
         _style_edit_update();
         CRIT_ON_FAIL(editor_save(ap.project->global_object));
         ap.project->changed = true;
         elm_object_disabled_set(pa->action2.control, !tpd.ellipsis_check);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_FORMAT_ELLIPSIS_VALUE:
         tpd.ellipsis_value = double_val1;
         _style_edit_update();
         CRIT_ON_FAIL(editor_save(ap.project->global_object));
         ap.project->changed = true;
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_FORMAT_LINE_RELATED_SIZE:
         tpd.linerelsize = double_val1;
         _style_edit_update();
         CRIT_ON_FAIL(editor_save(ap.project->global_object));
         ap.project->changed = true;
         break;

      case ATTRIBUTE_TEXTBLOCK_ITEM_GLOW_SHADOW_STYLE:
         assert(cb_item != NULL);
         tpd.glow_style = cb_item->index;
         _style_edit_update();
         CRIT_ON_FAIL(editor_save(ap.project->global_object));
         ap.project->changed = true;
         GENLIST_FILTER_APPLY(pd->genlist);
         elm_object_text_set(action->control, font_glow_list[tpd.glow_style]);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_GLOW_SHADOW_COLOR:
         tpd.shadow_color.r = r;
         tpd.shadow_color.g = g;
         tpd.shadow_color.b = b;
         tpd.shadow_color.a = a;
         _style_edit_update();
         CRIT_ON_FAIL(editor_save(ap.project->global_object));
         ap.project->changed = true;
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_GLOW_SHADOW_DIRECTION:
         assert(cb_item != NULL);
         tpd.direction = cb_item->index;
         _style_edit_update();
         ap.project->changed = true;
         elm_object_text_set(action->control, direction_list[tpd.direction]);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_GLOW_SHADOW_OUTER_COLOR:
         tpd.outer_color.r = r;
         tpd.outer_color.g = g;
         tpd.outer_color.b = b;
         tpd.outer_color.a = a;
         _style_edit_update();
         CRIT_ON_FAIL(editor_save(ap.project->global_object));
         ap.project->changed = true;
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_GLOW_SHADOW_INNER_COLOR:
         tpd.inner_color.r = r;
         tpd.inner_color.g = g;
         tpd.inner_color.b = b;
         tpd.inner_color.a = a;
         _style_edit_update();
         CRIT_ON_FAIL(editor_save(ap.project->global_object));
         ap.project->changed = true;
         break;

      case ATTRIBUTE_TEXTBLOCK_ITEM_LINES_STRIKETHROUGH_CHECK:
         tpd.strikethrough_check = bool_val1;
         _style_edit_update();
         CRIT_ON_FAIL(editor_save(ap.project->global_object));
         ap.project->changed = true;
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_LINES_STRIKETHROUGH_COLOR:
         tpd.strikethrough_color.r = r;
         tpd.strikethrough_color.g = g;
         tpd.strikethrough_color.b = b;
         tpd.strikethrough_color.a = a;
         _style_edit_update();
         CRIT_ON_FAIL(editor_save(ap.project->global_object));
         ap.project->changed = true;
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_LINES_UNDERLINE:
         assert(cb_item != NULL);
         tpd.underline = cb_item->index;
         _style_edit_update();
         CRIT_ON_FAIL(editor_save(ap.project->global_object));
         ap.project->changed = true;
         GENLIST_FILTER_APPLY(pd->genlist);
         elm_object_text_set(action->control, underl_styles[tpd.underline]);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_LINES_UNDERLINE_COLOR_ONE:
         tpd.underone_color.r = r;
         tpd.underone_color.g = g;
         tpd.underone_color.b = b;
         tpd.underone_color.a = a;
         _style_edit_update();
         CRIT_ON_FAIL(editor_save(ap.project->global_object));
         ap.project->changed = true;
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_LINES_UNDERLINE_COLOR_TWO:
         tpd.undertwo_color.r = r;
         tpd.undertwo_color.g = g;
         tpd.undertwo_color.b = b;
         tpd.undertwo_color.a = a;
         _style_edit_update();
         CRIT_ON_FAIL(editor_save(ap.project->global_object));
         ap.project->changed = true;
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_LINES_UNDERLINE_DASH_WIDTH:
         tpd.dash_width = double_val1;
         _style_edit_update();
         CRIT_ON_FAIL(editor_save(ap.project->global_object));
         ap.project->changed = true;
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_LINES_UNDERLINE_DASH_GAP:
         tpd.dash_gap = double_val1;
         _style_edit_update();
         CRIT_ON_FAIL(editor_save(ap.project->global_object));
         ap.project->changed = true;
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_LINES_UNDERLINE_DASH_COLOR:
         tpd.dash_color.r = r;
         tpd.dash_color.g = g;
         tpd.dash_color.b = b;
         tpd.dash_color.a = a;
         _style_edit_update();
         CRIT_ON_FAIL(editor_save(ap.project->global_object));
         ap.project->changed = true;
         break;

      default:
         TODO("remove default case after all attributes will be added");
         CRIT("change callback not found for %s (%s)", pa->name, action->name ? action->name : "unnamed");
         abort();
         break;
     }
}

static Eina_Bool
_update_cb(Property_Attribute *pa, Property_Action *action)
{
   assert(pa != NULL);
   assert(action != NULL);
   assert(action->control != NULL);

   switch (action->type.attribute_textblock)
     {
      case ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_FONT_NAME:
         property_entry_set(action->control, tpd.font);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_FONT_STYLE_WEIGHT:
         elm_object_text_set(action->control, font_weight_list[tpd.font_style_weight]);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_FONT_STYLE_WIDTH:
         elm_object_text_set(action->control, font_width_list[tpd.font_style_width]);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_COLOR:
         property_color_control_color_set(action->control,
                                          tpd.color.r,
                                          tpd.color.g,
                                          tpd.color.b,
                                          tpd.color.a);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_SIZE:
         elm_spinner_value_set(action->control, tpd.font_size);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_POSITION_ALIGN_HOR:
         elm_object_text_set(action->control, font_horizontal_align[tpd.font_align_hor]);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_POSITION_ALIGN_VER:
         elm_object_text_set(action->control, font_horizontal_valign[tpd.font_valign_hor]);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_POSITION_MARGIN_LEFT:
         elm_spinner_value_set(action->control, tpd.left_margin);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_POSITION_MARGIN_RIGHT:
         elm_spinner_value_set(action->control, tpd.right_margin);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_POSITION_WRAP:
         elm_object_text_set(action->control, text_wrap[tpd.wrap]);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_FORMAT_TABSTOPS:
         elm_spinner_value_set(action->control, tpd.tabstops);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_FORMAT_LINE_SIZE:
         elm_spinner_value_set(action->control, tpd.linesize);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_FORMAT_BG_COLOR_CHECK:
         elm_check_state_set(action->control, tpd.bg_check);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_FORMAT_BG_COLOR_COLOR:
         property_color_control_color_set(action->control,
                                          tpd.bg_color.r,
                                          tpd.bg_color.g,
                                          tpd.bg_color.b,
                                          tpd.bg_color.a);
         elm_object_disabled_set(action->control, !tpd.bg_check);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_FORMAT_PASSWORD:
         elm_check_state_set(action->control, tpd.pass);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_FORMAT_ELLIPSIS_CHECK:
         elm_check_state_set(action->control, tpd.ellipsis_check);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_FORMAT_ELLIPSIS_VALUE:
         elm_object_disabled_set(action->control, !tpd.ellipsis_check);
         elm_spinner_value_set(action->control, tpd.ellipsis_value);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_FORMAT_LINE_RELATED_SIZE:
         elm_spinner_value_set(action->control, tpd.linerelsize);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_GLOW_SHADOW_STYLE:
         elm_object_text_set(action->control, font_glow_list[tpd.glow_style]);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_GLOW_SHADOW_COLOR:
         property_color_control_color_set(action->control,
                                          tpd.shadow_color.r,
                                          tpd.shadow_color.g,
                                          tpd.shadow_color.b,
                                          tpd.shadow_color.a);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_GLOW_SHADOW_DIRECTION:
         elm_object_text_set(action->control, direction_list[tpd.direction]);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_GLOW_SHADOW_OUTER_COLOR:
         property_color_control_color_set(action->control,
                                          tpd.outer_color.r,
                                          tpd.outer_color.g,
                                          tpd.outer_color.b,
                                          tpd.outer_color.a);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_GLOW_SHADOW_INNER_COLOR:
         property_color_control_color_set(action->control,
                                          tpd.inner_color.r,
                                          tpd.inner_color.g,
                                          tpd.inner_color.b,
                                          tpd.inner_color.a);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_LINES_STRIKETHROUGH_CHECK:
         elm_check_state_set(action->control, tpd.strikethrough_check);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_LINES_STRIKETHROUGH_COLOR:
         property_color_control_color_set(action->control,
                                          tpd.strikethrough_color.r,
                                          tpd.strikethrough_color.g,
                                          tpd.strikethrough_color.b,
                                          tpd.strikethrough_color.a);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_LINES_UNDERLINE:
         elm_object_text_set(action->control, underl_styles[tpd.underline]);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_LINES_UNDERLINE_COLOR_ONE:
         property_color_control_color_set(action->control,
                                          tpd.underone_color.r,
                                          tpd.underone_color.g,
                                          tpd.underone_color.b,
                                          tpd.underone_color.a);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_LINES_UNDERLINE_COLOR_TWO:
         property_color_control_color_set(action->control,
                                          tpd.undertwo_color.r,
                                          tpd.undertwo_color.g,
                                          tpd.undertwo_color.b,
                                          tpd.undertwo_color.a);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_LINES_UNDERLINE_DASH_WIDTH:
         elm_spinner_value_set(action->control, tpd.dash_width);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_LINES_UNDERLINE_DASH_GAP:
         elm_spinner_value_set(action->control, tpd.dash_gap);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_LINES_UNDERLINE_DASH_COLOR:
         property_color_control_color_set(action->control,
                                          tpd.dash_color.r,
                                          tpd.dash_color.g,
                                          tpd.dash_color.b,
                                          tpd.dash_color.a);
         break;
      default:
         TODO("remove default case after all attributes will be added");
         CRIT("update callback not found for %s (%s)", pa->name, action->name ? action->name : "unnamed");
         abort();
         break;
     }
   return true;
}

static void
_init_cb(Property_Attribute *pa, Property_Action *action)
{
   assert(pa != NULL);
   assert(action != NULL);
   assert(action->control != NULL);

   switch (action->type.attribute_textblock)
     {
      case ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_FONT_NAME:
         elm_entry_markup_filter_append(action->control,
                                        elm_entry_filter_accept_set,
                                        &accept_set);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_COLOR:
      case ATTRIBUTE_TEXTBLOCK_ITEM_FORMAT_BG_COLOR_CHECK:
      case ATTRIBUTE_TEXTBLOCK_ITEM_FORMAT_PASSWORD:
      case ATTRIBUTE_TEXTBLOCK_ITEM_FORMAT_ELLIPSIS_CHECK:
      case ATTRIBUTE_TEXTBLOCK_ITEM_FORMAT_BG_COLOR_COLOR:
      case ATTRIBUTE_TEXTBLOCK_ITEM_GLOW_SHADOW_COLOR:
      case ATTRIBUTE_TEXTBLOCK_ITEM_GLOW_SHADOW_OUTER_COLOR:
      case ATTRIBUTE_TEXTBLOCK_ITEM_GLOW_SHADOW_INNER_COLOR:
      case ATTRIBUTE_TEXTBLOCK_ITEM_LINES_STRIKETHROUGH_CHECK:
      case ATTRIBUTE_TEXTBLOCK_ITEM_LINES_STRIKETHROUGH_COLOR:
      case ATTRIBUTE_TEXTBLOCK_ITEM_LINES_UNDERLINE_COLOR_ONE:
      case ATTRIBUTE_TEXTBLOCK_ITEM_LINES_UNDERLINE_COLOR_TWO:
      case ATTRIBUTE_TEXTBLOCK_ITEM_LINES_UNDERLINE_DASH_COLOR:
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_FONT_STYLE_WEIGHT:
         _fill_combobox_with_enum(action->control, font_weight_list);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_FONT_STYLE_WIDTH:
         _fill_combobox_with_enum(action->control, font_width_list);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_SIZE:
         /* limit font size because, for example, value 600 completely ruin edj
            file and project cannot be opened anymore for now */
         elm_spinner_min_max_set(action->control, 1, 255);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_POSITION_ALIGN_HOR:
         _fill_combobox_with_enum(action->control, font_horizontal_align);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_POSITION_ALIGN_VER:
         _fill_combobox_with_enum(action->control, font_horizontal_valign);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_POSITION_MARGIN_LEFT:
      case ATTRIBUTE_TEXTBLOCK_ITEM_POSITION_MARGIN_RIGHT:
      case ATTRIBUTE_TEXTBLOCK_ITEM_FORMAT_TABSTOPS:
      case ATTRIBUTE_TEXTBLOCK_ITEM_FORMAT_LINE_SIZE:
      case ATTRIBUTE_TEXTBLOCK_ITEM_FORMAT_LINE_RELATED_SIZE:
      case ATTRIBUTE_TEXTBLOCK_ITEM_LINES_UNDERLINE_DASH_WIDTH:
      case ATTRIBUTE_TEXTBLOCK_ITEM_LINES_UNDERLINE_DASH_GAP:
         elm_spinner_min_max_set(action->control, 0, 9999);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_POSITION_WRAP:
         _fill_combobox_with_enum(action->control, text_wrap);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_FORMAT_ELLIPSIS_VALUE:
         /* because ellipsis SIGSEV on a lot of values sadly... */
         elm_spinner_min_max_set(action->control, 0, 100);
         elm_spinner_step_set(action->control, 1);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_GLOW_SHADOW_STYLE:
         _fill_combobox_with_enum(action->control, font_glow_list);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_GLOW_SHADOW_DIRECTION:
         _fill_combobox_with_enum(action->control, direction_list);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_LINES_UNDERLINE:
         _fill_combobox_with_enum(action->control, underl_styles);
         break;
      default:
         TODO("remove default case after all attributes will be added");
         CRIT("init callback not found for %s (%s)", pa->name, action->name ? action->name : "unnamed");
         abort();
         break;
     }
}

/* blocks */
static inline void
_action_internal(Property_Action *action, const char *name, const char *units,
                 Property_Control control_type, Attribute_Textblock_Item attribute,
                 const char *tooltip)
{
   /* check that we haven't inited this action already */
   assert(action->update_cb == NULL);

   action->name = name;
   action->units = units;
   action->tooltip = eina_stringshare_add(tooltip);
   action->control_type = control_type;
   action->type.attribute_textblock = attribute;
   action->init_cb = _init_cb;
   action->update_cb = _update_cb;
   action->change_cb = _change_cb;
}

static inline void
_action1(Property_Attribute *pa, const char *name, const char *units,
         Property_Control control_type, Attribute_Textblock_Item attribute,
         const char *tooltip)
{
   _action_internal(&pa->action1, name, units, control_type, attribute, tooltip);

   assert(attribute_map[attribute].pa == NULL);
   assert(attribute_map[attribute].action == NULL);

   attribute_map[attribute].pa = pa;
   attribute_map[attribute].action = &pa->action1;
}

static inline void
_action2(Property_Attribute *pa, const char *name, const char *units,
         Property_Control control_type, Attribute_Textblock_Item attribute,
         const char *tooltip)
{
   _action_internal(&pa->action2, name, units, control_type, attribute, tooltip);

   assert(attribute_map[attribute].pa == NULL);
   assert(attribute_map[attribute].action == NULL);

   attribute_map[attribute].pa = pa;
   attribute_map[attribute].action = &pa->action2;
}

static Eina_Bool
_filter_cb(Property_Attribute *pa)
{
   assert(pa != NULL);

   /* will be updated later on */
   return tpd.selected;
}

static Eina_Bool
_direction_filter_cb(Property_Attribute *pa)
{
   assert(pa != NULL);

   return !!strstr(font_glow_list[tpd.glow_style], "shadow");
}

static Eina_Bool
_no_direction_filter_cb(Property_Attribute *pa)
{
   assert(pa != NULL);

   return !strstr(font_glow_list[tpd.glow_style], "shadow");
}

static Eina_Bool
_underline_filter_cb(Property_Attribute *pa)
{
   assert(pa != NULL);

   if ((pa->type.textblock_item == PROPERTY_TEXTBLOCK_ITEM_LINES_UNDERLINE_DASH) ||
      (pa->type.textblock_item == PROPERTY_TEXTBLOCK_ITEM_LINES_UNDERLINE_DASH_COLOR))
     return tpd.underline == DASHED_UNDERLINE_NUMBER;
   else if (pa->type.textblock_item == PROPERTY_TEXTBLOCK_ITEM_LINES_UNDERLINE_COLOR_TWO)
     return tpd.underline == DOUBLE_UNDERLINE_NUMBER;
   else
     return (tpd.underline < DASHED_UNDERLINE_NUMBER) && (tpd.underline);
}

static void
_init_items()
{
   Property_Textblock_Item it;
#define IT textblock_pd.items[it]
   for (it = 0 /* first element of enum */; it < PROPERTY_TEXTBLOCK_ITEM_LAST; it++)
     {
        IT.type.textblock_item = it;
        IT.filter_cb = _filter_cb;

        /* default value: show for all types */
        IT.filter_data.textblock_types = ~0u;
        switch(it)
          {
           case PROPERTY_TEXTBLOCK_ITEM_TEXT_TITLE:
              IT.name = "Text";
              IT.expandable = true;
              IT.expanded = true;
              IT.expand_cb = _subitems_get;
              break;
           case PROPERTY_TEXTBLOCK_ITEM_TEXT_FONT_NAME:
              IT.name = "Font name";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_ENTRY, ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_FONT_NAME,
                       _("Font description in fontconfig format. "
                         "For example, \"Sans:style=Italic:lang=hi\" or \"Serif:style=Bold\"."
                         "Warning! All font names shouldn't contain spaces."
                         "Font name \"Times New Roman\" should be written as \"TimesNewRoman\"!"));
              break;
           case PROPERTY_TEXTBLOCK_ITEM_TEXT_FONT_STYLE_WEIGHT:
              IT.name = "Font style (weight)";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_COMBOBOX, ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_FONT_STYLE_WEIGHT,
                       _("Overrides the weight defined in \"font\". "
                         "For example, the value \"Bold\" is the "
                         "same as font value \"style=Bold\"."));
              break;
           case PROPERTY_TEXTBLOCK_ITEM_TEXT_FONT_STYLE_WIDTH:
              IT.name = "Font style (width)";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_COMBOBOX, ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_FONT_STYLE_WIDTH,
                       _("Overrides the width defined in \"font\". "
                         "For example, the value \"Condensed\" is the "
                         "same as font value \"style=Condensed\"."));
              break;
           case PROPERTY_TEXTBLOCK_ITEM_TEXT_COLOR:
              IT.name = "Color";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_COLOR, ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_COLOR,
                       _("Text color. Color can be chosen "
                         "using the color selector UI tool."));
              break;
           case PROPERTY_TEXTBLOCK_ITEM_TEXT_SIZE:
              IT.name = "Size";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_SPINNER, ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_SIZE,
                       _("The font size in points."));
              break;

           case PROPERTY_TEXTBLOCK_ITEM_POSITION_TITLE:
              IT.name = "Positioning options";
              IT.expandable = true;
              IT.expanded = true;
              IT.expand_cb = _subitems_get;
              break;
           case PROPERTY_TEXTBLOCK_ITEM_POSITION_ALIGN_HOR:
              IT.name = "Horizontal align";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_COMBOBOX, ATTRIBUTE_TEXTBLOCK_ITEM_POSITION_ALIGN_HOR,
                      _("Sets the horizontal alignment of the text.<br>"
                        " * auto  - Respects LTR/RTL settings.<br> "
                        " * center - Centers the text in the line.<br>"
                        " * middle - Alias for \"center\"<br> "
                        " * left  - Aligns the text left.<br>"
                        " * right  - Aligns the text right.<br>"));
              break;
           case PROPERTY_TEXTBLOCK_ITEM_POSITION_ALIGN_VER:
              IT.name = "Vertical align";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_COMBOBOX, ATTRIBUTE_TEXTBLOCK_ITEM_POSITION_ALIGN_VER,
                       _("Sets the vertical alignment of the text.<br>"
                         " * top - Puts text at the top of the line.<br>"
                         " * center  - Centers the text in the line.<br>"
                         " * middle  - Alias for \"center\"<br>"
                         " * bottom  - Puts the text at the bottom of the line.<br>"
                         " * baseline - Baseline<br>"
                         " * base - Alias for \"baseline\""));
              break;
           case PROPERTY_TEXTBLOCK_ITEM_POSITION_MARGIN:
              IT.name = "Margin";
              _action1(&IT, "left:", "px", PROPERTY_CONTROL_SPINNER, ATTRIBUTE_TEXTBLOCK_ITEM_POSITION_MARGIN_LEFT,
                       _("Sets the left margin of the text in pixel."));
              _action2(&IT, "right:", "px", PROPERTY_CONTROL_SPINNER, ATTRIBUTE_TEXTBLOCK_ITEM_POSITION_MARGIN_RIGHT,
                       _("Sets the right margin of the text in pixel."));
              break;
           case PROPERTY_TEXTBLOCK_ITEM_POSITION_WRAP:
              IT.name = "Wrap";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_COMBOBOX, ATTRIBUTE_TEXTBLOCK_ITEM_POSITION_WRAP,
                       _("Sets the wrap policy of the text.<br>"
                         " * word  - Only wraps lines at word boundaries.<br>"
                         " * char  - Wraps at any character.<br>"
                         " * mixed - Wrap at words if possible, if not, at any character.<br>"
                         " * hyphenation - Hyphenate if possible, if not, wrap at words if possible,<br>"
                         "                 if not, at any character."));
              break;

           case PROPERTY_TEXTBLOCK_ITEM_FORMAT_TITLE:
              IT.name = "Text format";
              IT.expandable = true;
              IT.expanded = true;
              IT.expand_cb = _subitems_get;
              break;
           case PROPERTY_TEXTBLOCK_ITEM_FORMAT_TABSTOPS:
              IT.name = "Tabstops";
              _action1(&IT, NULL, "px", PROPERTY_CONTROL_SPINNER, ATTRIBUTE_TEXTBLOCK_ITEM_FORMAT_TABSTOPS,
                       _("Sets the size of the tab character. "
                         "The value must be a number greater than 1."));
              break;
           case PROPERTY_TEXTBLOCK_ITEM_FORMAT_LINE_SIZE:
              IT.name = "Line size";
              _action1(&IT, NULL, "px", PROPERTY_CONTROL_SPINNER, ATTRIBUTE_TEXTBLOCK_ITEM_FORMAT_LINE_SIZE,
                       _("Sets the size of line of text."));
              break;
           case PROPERTY_TEXTBLOCK_ITEM_FORMAT_BG_COLOR:
              IT.name = "Background color";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_CHECK, ATTRIBUTE_TEXTBLOCK_ITEM_FORMAT_BG_COLOR_CHECK,
                       _("Enables or disables the background color."));
              _action2(&IT, NULL, NULL, PROPERTY_CONTROL_COLOR, ATTRIBUTE_TEXTBLOCK_ITEM_FORMAT_BG_COLOR_COLOR,
                       _("Sets a background color for text."));
              break;
           case PROPERTY_TEXTBLOCK_ITEM_FORMAT_PASSWORD:
              IT.name = "Password field";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_CHECK, ATTRIBUTE_TEXTBLOCK_ITEM_FORMAT_PASSWORD,
                       _("Sets if the text is being used for passwords. "
                         "Enabling this causes all characters to be substituted for '*'."));
              break;
           case PROPERTY_TEXTBLOCK_ITEM_FORMAT_ELLIPSIS:
              IT.name = "Ellipsis";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_CHECK, ATTRIBUTE_TEXTBLOCK_ITEM_FORMAT_ELLIPSIS_CHECK,
                       _("Enables or disables the ellipsis feature for text."));
              _action2(&IT, NULL, "%", PROPERTY_CONTROL_SPINNER, ATTRIBUTE_TEXTBLOCK_ITEM_FORMAT_ELLIPSIS_VALUE,
                       _("Sets the ellipsis mode. A value of 0 means ellipsizing "
                         "the leftmost portion of the text first, "
                         "1 on the other hand the rightmost portion."));
              break;
           case PROPERTY_TEXTBLOCK_ITEM_FORMAT_LINE_RELATED_SIZE:
              IT.name = "Line related size";
              _action1(&IT, NULL, "px", PROPERTY_CONTROL_SPINNER, ATTRIBUTE_TEXTBLOCK_ITEM_FORMAT_LINE_RELATED_SIZE,
                       _("A floating point value indicating the wanted size "
                         "of the line relative to the calculated size."));
              break;

           case PROPERTY_TEXTBLOCK_ITEM_GLOW_SHADOW_TITLE:
              IT.name = "Glow & Shadow";
              IT.expandable = true;
              IT.expanded = true;
              IT.expand_cb = _subitems_get;
              break;
           case PROPERTY_TEXTBLOCK_ITEM_GLOW_SHADOW_STYLE:
              IT.name = "Style";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_COMBOBOX, ATTRIBUTE_TEXTBLOCK_ITEM_GLOW_SHADOW_STYLE,
                       _("Sets the style of the text."));
              break;
           case PROPERTY_TEXTBLOCK_ITEM_GLOW_SHADOW_COLOR:
              IT.name = "Color";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_COLOR, ATTRIBUTE_TEXTBLOCK_ITEM_GLOW_SHADOW_COLOR,
                       _("Sets the color of the text shadow."));
              IT.filter_cb = _direction_filter_cb;
              break;
           case PROPERTY_TEXTBLOCK_ITEM_GLOW_SHADOW_DIRECTION:
              IT.name = "Direction";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_COMBOBOX, ATTRIBUTE_TEXTBLOCK_ITEM_GLOW_SHADOW_DIRECTION,
                       _("Sets the shadow direction"));
              IT.filter_cb = _direction_filter_cb;
              break;
           case PROPERTY_TEXTBLOCK_ITEM_GLOW_SHADOW_OUTER_COLOR:
              IT.name = "Outer glow color";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_COLOR, ATTRIBUTE_TEXTBLOCK_ITEM_GLOW_SHADOW_OUTER_COLOR,
                       _("Sets the outer shadow glow color."));
              IT.filter_cb = _no_direction_filter_cb;
              break;
           case PROPERTY_TEXTBLOCK_ITEM_GLOW_SHADOW_INNER_COLOR:
              IT.name = "Inner glow color";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_COLOR, ATTRIBUTE_TEXTBLOCK_ITEM_GLOW_SHADOW_INNER_COLOR,
                       _("Sets the inner shadow glow color."));
              IT.filter_cb = _no_direction_filter_cb;
              break;

           case PROPERTY_TEXTBLOCK_ITEM_LINES_TITLE:
              IT.name = "Lines";
              IT.expandable = true;
              IT.expanded = true;
              IT.expand_cb = _subitems_get;
              break;
           case PROPERTY_TEXTBLOCK_ITEM_LINES_STRIKETHROUGH:
              IT.name = "Strikethrough color";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_CHECK, ATTRIBUTE_TEXTBLOCK_ITEM_LINES_STRIKETHROUGH_CHECK,
                       _("Sets if the text is striked through."));
              _action2(&IT, NULL, NULL, PROPERTY_CONTROL_COLOR, ATTRIBUTE_TEXTBLOCK_ITEM_LINES_STRIKETHROUGH_COLOR,
                       _("Sets the color of the text that is striked through."));
              break;
           case PROPERTY_TEXTBLOCK_ITEM_LINES_UNDERLINE:
              IT.name = "Underline";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_COMBOBOX, ATTRIBUTE_TEXTBLOCK_ITEM_LINES_UNDERLINE,
                       _("Sets if and how a text is underlined."));
              break;
           case PROPERTY_TEXTBLOCK_ITEM_LINES_UNDERLINE_COLOR_ONE:
              IT.name = "Underline color one";
              IT.filter_cb = _underline_filter_cb;
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_COLOR, ATTRIBUTE_TEXTBLOCK_ITEM_LINES_UNDERLINE_COLOR_ONE,
                       _("Sets the color of the underline."));
              break;
           case PROPERTY_TEXTBLOCK_ITEM_LINES_UNDERLINE_COLOR_TWO:
              IT.name = "Underline color two";
              IT.filter_cb = _underline_filter_cb;
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_COLOR, ATTRIBUTE_TEXTBLOCK_ITEM_LINES_UNDERLINE_COLOR_TWO,
                       _("Sets the color of the second line of the underline "
                         "when using the \"double\" underline mode."));
              break;
           case PROPERTY_TEXTBLOCK_ITEM_LINES_UNDERLINE_DASH:
              IT.name = "Underline dash";
              IT.filter_cb = _underline_filter_cb;
              _action1(&IT, "width", NULL, PROPERTY_CONTROL_SPINNER, ATTRIBUTE_TEXTBLOCK_ITEM_LINES_UNDERLINE_DASH_WIDTH,
                       _("Sets the width of the underline dash."));
              _action2(&IT, "gap", NULL, PROPERTY_CONTROL_SPINNER, ATTRIBUTE_TEXTBLOCK_ITEM_LINES_UNDERLINE_DASH_GAP,
                       _("Sets the gap of the underline dash."));
              break;
           case PROPERTY_TEXTBLOCK_ITEM_LINES_UNDERLINE_DASH_COLOR:
              IT.name = "Underline dash color";
              IT.filter_cb = _underline_filter_cb;
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_COLOR, ATTRIBUTE_TEXTBLOCK_ITEM_LINES_UNDERLINE_DASH_COLOR,
                       _("Sets the color of dashed underline."));
              break;

           case PROPERTY_TEXTBLOCK_ITEM_LAST:
              break;
          }
     }
#undef IT
}

static void
_on_style_selected(void *data,
                   Evas_Object *obj __UNUSED__,
                   void *event_info)
{
   Property_Data *pd = (Property_Data *)data;
   Style_Data *cur_style = (Style_Data *)event_info;
   Eina_Tmpstr *tmp;
   const char *value;
   char *style_copy = NULL;
   char *token;
   int count = 0, i = 0;

   if (cur_style)
     {
        /* do a split */
        tpd.selected = true;
        tpd.current_style = *((Style_Data *) cur_style);
        value = eina_strbuf_string_get(cur_style->style);

        /* working with first subitems of TEXT */
        tmp = _tag_value_get(value, "font");
        if (!tmp) tmp = eina_tmpstr_add("");
        if (tpd.font) eina_stringshare_del(tpd.font);
        tpd.font = eina_stringshare_add(tmp);
        eina_tmpstr_del(tmp);

        tmp = _tag_value_get(value, "font_size");
        if (!tmp) tmp = eina_tmpstr_add("24");
        tpd.font_size = atof(tmp);
        eina_tmpstr_del(tmp);

        tmp = _tag_value_get(value, "font_weight");
        if (!tmp) tmp = eina_tmpstr_add("Normal");
        tpd.font_style_weight = _combobox_get_num(tmp, font_weight_list);
        eina_tmpstr_del(tmp);

        tmp = _tag_value_get(value, "font_width");
        if (!tmp) tmp = eina_tmpstr_add("Normal");
        tpd.font_style_width = _combobox_get_num(tmp, font_width_list);
        eina_tmpstr_del(tmp);

        tmp = _tag_value_get(value, "color");
        if (!_hex_to_rgb(tmp, &tpd.color.r, &tpd.color.g, &tpd.color.b, &tpd.color.a))
          {
             ERR("Can't convert color value");
             abort();
          }
        eina_tmpstr_del(tmp);

        /* working with second subitems of POSITIONS */
        tmp = _tag_value_get(value, "align");
        if (!tmp) tmp = eina_tmpstr_add("auto");
        tpd.font_align_hor = _combobox_get_num(tmp, font_horizontal_align);
        eina_tmpstr_del(tmp);

        tmp = _tag_value_get(value, "valign");
        if (!tmp) tmp = eina_tmpstr_add("baseline");
        tpd.font_valign_hor = _combobox_get_num(tmp, font_horizontal_valign);
        eina_tmpstr_del(tmp);

        tmp = _tag_value_get(value, "left_margin");
        if (!tmp) tmp = eina_tmpstr_add("0");
        tpd.left_margin = atof(tmp);
        eina_tmpstr_del(tmp);

        tmp = _tag_value_get(value, "right_margin");
        if (!tmp) tmp = eina_tmpstr_add("0");
        tpd.right_margin = atof(tmp);
        eina_tmpstr_del(tmp);

        tmp = _tag_value_get(value, "wrap");
        if (!tmp) tmp = eina_tmpstr_add("none");
        tpd.wrap = _combobox_get_num(tmp, text_wrap);
        eina_tmpstr_del(tmp);

        /* working with FORMAT */
        tmp = _tag_value_get(value, "tabstops");
        if (!tmp) tmp = eina_tmpstr_add("0");
        tpd.tabstops = atof(tmp);
        eina_tmpstr_del(tmp);

        tmp = _tag_value_get(value, "linesize");
        if (!tmp) tmp = eina_tmpstr_add("0");
        tpd.linesize = atof(tmp);
        eina_tmpstr_del(tmp);

        tmp = _tag_value_get(value, "backing");
        if ((!tmp) || (!strcmp(tmp, "off")))
          tpd.bg_check = EINA_FALSE;
        else
          tpd.bg_check = EINA_TRUE;
        eina_tmpstr_del(tmp);
        tmp = _tag_value_get(value, "backing_color");
        if (!tmp)
          tmp = eina_tmpstr_add(WHITE_COLOR);
        if (!_hex_to_rgb(tmp,
                         &tpd.bg_color.r,
                         &tpd.bg_color.g,
                         &tpd.bg_color.b,
                         &tpd.bg_color.a))
          {
             ERR("Can't convert backgorund color value");
             abort();
          }
        eina_tmpstr_del(tmp);

        tmp = _tag_value_get(value, "password");
        if ((!tmp) || (!strcmp(tmp, "off")))
          tpd.pass = false;
        else
          tpd.pass = true;
        eina_tmpstr_del(tmp);

        tmp = _tag_value_get(value, "ellipsis");
        if (!tmp) tmp = eina_tmpstr_add("0");
        tpd.ellipsis_value = atof(tmp);
        eina_tmpstr_del(tmp);

        tmp = _tag_value_get(value, "check_ellipsis");
        if ((!tmp) || (!strcmp(tmp, "off")))
          {
             tpd.ellipsis_value = -100;
             tpd.ellipsis_check = false;
          }
        else
          tpd.ellipsis_check = true;
        eina_tmpstr_del(tmp);

        tmp = _tag_value_get(value, "linerelsize");
        if (!tmp) tmp = eina_tmpstr_add("0");
        tpd.linerelsize = atof(tmp);
        eina_tmpstr_del(tmp);

        /* working with wonderful glow and shadow now */
        tmp = _tag_value_get(value, "style");
        if (!tmp) tmp = eina_tmpstr_add("none");
        style_copy = mem_malloc(strlen_safe(tmp) + 1);
        strcpy(style_copy, tmp);
        token = strtok(style_copy, ",");
        TODO("replace with eina_str_split_full")
        while (token)
          {
             if (count == 0)
               {
                  tmp = eina_tmpstr_add(token);
                  count++;
               }
             else
               {
                  for (i = 0; direction_list[i] != NULL; i++)
                    if (!strcmp(direction_list[i], token))
                      tpd.direction = i;
               }
             token = strtok(0, " ");
          }
        free(style_copy);
        if (!_check_value(font_glow_list, tmp))
          {
             eina_tmpstr_del(tmp);
             tmp = eina_tmpstr_add("none");
          }
        tpd.glow_style = _combobox_get_num(tmp, font_glow_list);
        eina_tmpstr_del(tmp);

        tmp = _tag_value_get(value, "glow_color");
        if (!tmp)
          tmp = eina_tmpstr_add(WHITE_COLOR);
        if (!_hex_to_rgb(tmp,
                         &tpd.inner_color.r,
                         &tpd.inner_color.g,
                         &tpd.inner_color.b,
                         &tpd.inner_color.a))
          {
             ERR("Can't convert backgorund color value");
             abort();
          }
        eina_tmpstr_del(tmp);

        tmp = _tag_value_get(value, "glow2_color");
        if (!tmp)
          tmp = eina_tmpstr_add(WHITE_COLOR);
        if (!_hex_to_rgb(tmp,
                         &tpd.outer_color.r,
                         &tpd.outer_color.g,
                         &tpd.outer_color.b,
                         &tpd.outer_color.a))
          {
             ERR("Can't convert backgorund color value");
             abort();
          }
        eina_tmpstr_del(tmp);

        tmp = _tag_value_get(value, "shadow_color");
        if (!tmp)
          tmp = eina_tmpstr_add(WHITE_COLOR);
        if (!_hex_to_rgb(tmp,
                         &tpd.shadow_color.r,
                         &tpd.shadow_color.g,
                         &tpd.shadow_color.b,
                         &tpd.shadow_color.a))
          {
             ERR("Can't convert backgorund color value");
             abort();
          }
        eina_tmpstr_del(tmp);

        /* and finally LINES */
        tmp = _tag_value_get(value, "strikethrough");
        if ((!tmp) || (!strcmp(tmp, "off")))
          tpd.strikethrough_check = false;
        else
          tpd.strikethrough_check = true;
        eina_tmpstr_del(tmp);
        tmp = _tag_value_get(value, "strikethrough_color");
        if (!tmp)
          tmp = eina_tmpstr_add(WHITE_COLOR);
        if (!_hex_to_rgb(tmp,
                         &tpd.strikethrough_color.r,
                         &tpd.strikethrough_color.g,
                         &tpd.strikethrough_color.b,
                         &tpd.strikethrough_color.a))
          {
             ERR("Can't convert backgorund color value");
             abort();
          }
        eina_tmpstr_del(tmp);

        tmp = _tag_value_get(value, "underline");
        if (!tmp) tmp = eina_tmpstr_add("off");
        else if (!strcmp(tmp, "on")) /* check useless alias */
          {
             eina_tmpstr_del(tmp);
             tmp = eina_tmpstr_add("single");
          }
        tpd.underline = _combobox_get_num(tmp, underl_styles);
        eina_tmpstr_del(tmp);

        tmp = _tag_value_get(value, "underline_color");
        if (!tmp)
          tmp = eina_tmpstr_add(WHITE_COLOR);
        if (!_hex_to_rgb(tmp,
                         &tpd.underone_color.r,
                         &tpd.underone_color.g,
                         &tpd.underone_color.b,
                         &tpd.underone_color.a))
          {
             ERR("Can't convert underline color value");
             abort();
          }
        eina_tmpstr_del(tmp);

        tmp = _tag_value_get(value, "underline2_color");
        if (!tmp)
          tmp = eina_tmpstr_add(WHITE_COLOR);
        if (!_hex_to_rgb(tmp,
                         &tpd.undertwo_color.r,
                         &tpd.undertwo_color.g,
                         &tpd.undertwo_color.b,
                         &tpd.undertwo_color.a))
          {
             ERR("Can't convert underline2 color value");
             abort();
          }
        eina_tmpstr_del(tmp);

        tmp = _tag_value_get(value, "underline_dash_width");
        if (!tmp) tmp = eina_tmpstr_add("0");
        tpd.dash_width = atof(tmp);
        eina_tmpstr_del(tmp);

        tmp = _tag_value_get(value, "underline_dash_gap");
        if (!tmp) tmp = eina_tmpstr_add("0");
        tpd.dash_gap = atof(tmp);
        eina_tmpstr_del(tmp);

        tmp = _tag_value_get(value, "underline_dash_color");
        if (!tmp)
          tmp = eina_tmpstr_add(WHITE_COLOR);
        if (!_hex_to_rgb(tmp,
                         &tpd.dash_color.r,
                         &tpd.dash_color.g,
                         &tpd.dash_color.b,
                         &tpd.dash_color.a))
          {
             ERR("Can't convert underline2 color value");
             abort();
          }
        eina_tmpstr_del(tmp);
     }
   else
     {
        tpd.selected = false;
        if (tpd.font)
          eina_stringshare_del(tpd.font);
        tpd.font = NULL;
     }

   GENLIST_FILTER_APPLY(pd->genlist);
   property_item_update_recursively(&textblock_pd.items[PROPERTY_TEXTBLOCK_ITEM_TEXT_TITLE]);
   property_item_update_recursively(&textblock_pd.items[PROPERTY_TEXTBLOCK_ITEM_POSITION_TITLE]);
   property_item_update_recursively(&textblock_pd.items[PROPERTY_TEXTBLOCK_ITEM_FORMAT_TITLE]);
   property_item_update_recursively(&textblock_pd.items[PROPERTY_TEXTBLOCK_ITEM_GLOW_SHADOW_TITLE]);
   property_item_update_recursively(&textblock_pd.items[PROPERTY_TEXTBLOCK_ITEM_LINES_TITLE]);
}

#undef tpd

/* public */
void
property_textblock_manager_init(Property_Data *pd)
{
   assert(pd != NULL);

   _init_items();

   evas_object_smart_callback_add(ap.win, SIGNAL_STYLE_SELECTED, _on_style_selected, pd);
}

Eina_List *
property_textblock_manager_items_get()
{
   Eina_List *items = NULL;

   items = eina_list_append(items, &textblock_pd.items[PROPERTY_TEXTBLOCK_ITEM_TEXT_TITLE]);
   items = eina_list_append(items, &textblock_pd.items[PROPERTY_TEXTBLOCK_ITEM_POSITION_TITLE]);
   items = eina_list_append(items, &textblock_pd.items[PROPERTY_TEXTBLOCK_ITEM_FORMAT_TITLE]);
   items = eina_list_append(items, &textblock_pd.items[PROPERTY_TEXTBLOCK_ITEM_GLOW_SHADOW_TITLE]);
   items = eina_list_append(items, &textblock_pd.items[PROPERTY_TEXTBLOCK_ITEM_LINES_TITLE]);

   return items;
}

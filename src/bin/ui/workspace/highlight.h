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

#ifndef HIGHLIGHT_H
#define HIGHLIGHT_H

#include "eflete.h"

TODO("THIS PART IS STILL BEING DEVELOPED. MAKE t, l, r, b and cc parts please!")
/**
 * @defgroup Highlight Highlight
 * @ingroup Workspace
 *
 * This highlight is needed for highlighting the current part, that was
 * selected by user in the list of parts that is on the left part
 * of the main window.
 *
 * Basically highlight consist from 12 parts. 9 of it are handlers.
 * handlers: lt (Left-Top handler), lb (Left-Bottom handler),
 * rt (Right-Top handler), rb (Right-Bottom handler), l (Left handler),
 * r (Right), b (Bottom), t (Top), cc (Center handler).
 *
 * All handlers has size that is relative to the whole highlighted part. Size of
 * handlers width and height counts as COEFF * parts {H | W}. COEFF is defined
 * in C part and has value 0.25
 *
 * If highlighted part is too large, then handler will have fixed size. This size
 * also defined, and it's value is 200. That means that if
 * COEFF * partsH > MAXSIZE, then it automatically turns into MAXSIZE
 * (which is 200).
 *
 * This highlight highlights selected part, and user can see four
 * (lt, lb, rt, rb) handlers and by moving mouse on one of them (or on one of
 * unseen handlers) handler will be shown, but others will be hided.
 *
 * ---------------------
 * | lt |    t    | rt |
 * |-------------------|
 * |    |         |    |
 * |  l |    cc   | r  |
 * |    |         |    |
 * |-------------------|
 * | lb |    b    | rb |
 * ---------------------
 *
 * For example, here we have highlighted part:
 * ---------------------
 * | lt |         | rt |
 * |-----         -----|
 * |                   |
 * |                   |
 * |                   |
 * |-----         -----|
 * | lb |         | rb |
 * ---------------------
 *
 * By moving the mouse on or by clicking any handler by finger, highlight will
 * be looking like this:
 * ---------------------
 * |                   |
 * |                   |
 * |                   |
 * |                   |
 * |                   |
 * |    -----------    |
 * |    |    b    |    |
 * ---------------------
 *
 * Also, if the size of highlighted part is too small for inner handlers of
 * the highlight, those handlers will be positioned outside of the highlight
 * border. The default size (size, when highlighted part will be count as small)
 * is defined in see part, and basically it's 120 px. So, if width or height of
 * the highlighted part is lower than that MINSIZE, then handler's will be shown
 * outside of the highlighted part.
 * For example, default view of such highlight:
 * ------             ------
 * | lt |             | rt |
 * -------------------------
 *      | Highlighted |
 *      |     part    |
 * -------------------------
 * | lb |             | rb |
 * ------             ------
 *
 * By using handlers user can resize and move selected part.
 */

/**
 * @enum _Handler_Type
 *
 * @brief Type of the handler (RB - Right Bottom, TL - Top Left, etc)
 *
 * @ingroup WidgetManager
 */
enum _Handler_Type {
   RB,
   RT,
   LB,
   LT,
   MIDDLE
};

/**
 * @typedef Handler_Type
 * @ingroup Highlight
 */
typedef enum _Handler_Type Handler_Type;

/**
 * @struct _Highlight_Events
 *
 * @brief This struct designed to send in it's signal ("hl,changed") a new size
 * of highlight and send the type of handler (@see Handler_Type) that was used
 * to change it's size.
 *
 * @ingroup WidgetManager
 */
struct _Highlight_Events
{
   Evas_Coord x; /**< new X position of highlight */
   Evas_Coord y; /**< new Y position of highlight */
   Evas_Coord w; /**< new width of highlight */
   Evas_Coord h; /**< new height of highlight */
   Handler_Type descr; /**< type of handler that was using to change size and
                         position of highlight */
};
/**
 * @typedef Highlight_Events
 * @ingroup Highlight
 */
typedef struct _Highlight_Events Highlight_Events;

/**
 * @enum Highlight_Mode
 *
 * @brief Behaviour of Highlight handers.
 *
 * @ingroup Highlight
 */
typedef enum
{
   HIGHLIGHT_STATIC_HANDLERS,
   HIGHLIGHT_DYNAMIC_HANDLERS
} Highlight_Mode;

/**
 * Adds highlight.
 *
 * This function will initialize highlight object. It will create highlight
 * object, by loading edj file and then registrate all types of callbacks
 * like moving in and out of a handler and clicking on it.
 * The whole edj object will be resized to the size of groupspace
 * (for dragging a part within this area).
 *
 * @param parent The parent objectr, this should be a groupspace object.
 * @return New highlight object that has size of given groupspace (or any object
 * it will be working on). Returned object is Smart Object. So it can be resized,
 * moved, showed and hidden by using normal evas functions like, for example,
 * evas_object_show.
 *
 * @ingroup Highlight
 */
Evas_Object *
highlight_add(Evas_Object *parent);

/**
 * Set the object that created highlight object will highlight.
 * @note for future API!
 *
 * This function will set an object, so that highlight can resize and change
 * this object.
 *
 * @param highlight The smart object that is represent a highlight.
 * @param object The object that is represent selected part.
 * @return EINA_TRUE - successfull, EINA_FALSE - otherwise.
 *
 * @ingroup Highlight
 */
Eina_Bool
highlight_object_set(Evas_Object *hl, Evas_Object *obj);

/**
 * Hide middle handler and doesn't show in any way!
 *
 * @param highlight The smart object that is represent a highlight.
 * @return EINA_TRUE - successfull, EINA_FALSE - otherwise.
 *
 * @ingroup Highlight
 */
Eina_Bool
highlight_handler_align_hide(Evas_Object *hl);

/**
 * Show middle handler and doesn't show in any way!
 *
 * @param highlight The smart object that is represent a highlight.
 * @return EINA_TRUE - successfull, EINA_FALSE - otherwise.
 *
 * @ingroup Highlight
 */
Eina_Bool
highlight_handler_align_show(Evas_Object *hl);

/**
 * Get middle handler's visibility!
 *
 * @param highlight The smart object that is represent a highlight.
 * @return EINA_TRUE - successfull, EINA_FALSE - otherwise.
 *
 * @ingroup Highlight
 */
Eina_Bool
highlight_handler_align_visible_get(Evas_Object *hl);

/**
 * Disabled handlers of the highlight.
 *
 * This function will disable or enable showing handlers of the highlight.
 *
 * @param obj The smart object that is represent a highlight.
 * @param disabled EINA_TRUE will disable handlers, EINA_FALSE will enable handlers.
 * @return EINA_TRUE - successfull, EINA_FALSE - otherwise.
 *
 * @ingroup Highlight
 */
Eina_Bool
highlight_handler_disabled_set(Evas_Object *hl, Eina_Bool disabled);

/**
 * Set mode of the highlight.
 *
 * This function will change highlight mode into the given. Currently
 * there are only two modes:
 *
 * - HIGHLIGHT_STATIC_HANDLERS - when highlight is normal one, with handlers which has same size and placed outside of the main border.
 * - HIGHLIGHT_DYNAMIC_HANDLERS - when handlers are changing it's size depends on the size of main border. Handlers will be placed outside if the size small or inside if big enough.
 *
 * @param obj The smart object that is represent a highlight.
 * @param mode Mode of the highlight it will be changed into.
 * @return EINA_TRUE - successfull, EINA_FALSE - otherwise.
 *
 * @ingroup Highlight
 */
Eina_Bool
highlight_handler_mode_set(Evas_Object *hl, Highlight_Mode mode);

/**
 * Follow highlight to the given object.
 *
 * That mean that if the highlighted object will be changed (resized or moved)
 * highlight will be also changed to fit the object's new size.
 *
 * @param hl The smart object that is represent a highlight.
 * @param object Evas_Object that is being highlighted.
 * @return EINA_TRUE - successfull, EINA_FALSE - otherwise.
 *
 * @ingroup Highlight
 */
Eina_Bool
highlight_object_follow(Evas_Object *hl, Evas_Object *object);

/**
 * Unollow highlight from the given object.
 *
 * That mean that highlight is not following anymore.
 *
 * @param hl The smart object that is represent a highlight.
 * @return EINA_TRUE - successfull, EINA_FALSE - otherwise.
 *
 * @ingroup Highlight
 */
Eina_Bool
highlight_object_unfollow(Evas_Object *hl);

#endif /* HIGHLIGHT_H */

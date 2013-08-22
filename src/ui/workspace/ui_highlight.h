#ifndef HIGHLIGHT_H
#define HIGHLIGHT_H

#include <efl_tet.h>
#include "ui_groupspace.h"

/**
 * @defgroup Highlight Highlight
 *
 * TODO: THIS PART IS STILL BEING DEVELOPED.
 * TODO: "MAKE t, l, r, b and cc parts please!"
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

struct _Highlight_Events
{
   Evas_Coord x;
   Evas_Coord y;
   Evas_Coord w;
   Evas_Coord h;
};

typedef struct _Highlight_Events Highlight_Events;

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
hl_highlight_add(Evas_Object *parent);

/**
 * Set the object that created highlight object will highlight.
 * @note for future API!
 *
 * This function will set an object, so that highlight can resize and change
 * this object.
 *
 * @param highlight The smart object that is represent a highlight.
 * @param object The object that is represent selected part.
 *
 * @ingroup Highlight
 */
void
hl_highlight_object_set(Evas_Object *hl, Evas_Object *obj);

/**
 * Set the color of handlers of the highlight.
 *
 * This function will set given color to the border, that is used for showing
 * handlers.
 *
 * @param obj The smart object that is represent a highlight.
 * @param r Red color.
 * @param g Green color.
 * @param b Blur color.
 * @param a Alpha channel.
 *
 * @ingroup Highlight
 */
void
hl_highlight_handler_color_set(Evas_Object *hl,
                              Evas_Coord r,
                              Evas_Coord g,
                              Evas_Coord b,
                              Evas_Coord a);

/**
 * Set the color of border of the whole highlight.
 *
 * This function will set given color to the border, that is used for showing
 * highlight size.
 *
 * @param obj The smart object that is represent a highlight.
 * @param r Red color.
 * @param g Green color.
 * @param b Blur color.
 * @param a Alpha channel.
 *
 * @ingroup Highlight
 */
void
hl_highlight_border_color_set(Evas_Object *hl,
                              Evas_Coord r,
                              Evas_Coord g,
                              Evas_Coord b,
                              Evas_Coord a);

#endif /* HIGHLIGHT_H */

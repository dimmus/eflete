/* Elementary Widgets Extension
 * Copyright (C) 2014 Samsung Electronics.
 *
 * This file is part of Elementary Widgets Extension.
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
#ifndef EWE_RULER_LEGACY_H
#define EWE_RULER_LEGACY_H

/**
 * Add new ruler to the given parent object
 *
 * @param parent The parent object.
 *
 * @return a new ruler object or NULL if it cannot be created
 *
 * @ingroup Ruler
 */
Evas_Object *
ewe_ruler_add(Evas_Object *parent);

/**
 * Change the ruler orientation to horizontal or vertical.
 *
 * @param obj Ruler object.
 * @param horizontal The Ruler orientation.
 *
 * @return EINA_TRUE if orientation was set or EINA_FALSE on failure.
 * @ingroup Ruler
 */
Eina_Bool
ewe_ruler_horizontal_set(Evas_Object *obj, Eina_Bool horizontal);

/**
 * Get value of current ruler object orientation
 *
 * @param obj Ruler object.
 *
 * @return EINA_TRUE if ruler orientation is horizontal, EINA_FALSE otherwise
 *
 * or VERTICAL.
 *
 * @ingroup Ruler
 */
Eina_Bool
ewe_ruler_horizontal_get(const Eo *obj);

/**
 * Set step between marks in given scale of the ruler. Min step is 10.
 *
 * @param obj Ruler object.
 * @param scale Scale. NULL for default scale
 * @param step Step between marks.
 *
 * @return EINA_TRUE if step was set or EINA_FALSE on failure.
 *
 * @ingroup Ruler
 */
Eina_Bool
ewe_ruler_step_set(Evas_Object *obj, Ewe_Ruler_Scale * scale, unsigned int step);

/**
 * Get step between marks in given scale of the ruler
 *
 * @param obj Ruler object.
 * @param scale Scale. NULL for default scale
 *
 * @return value step between marks.
 *
 * @ingroup Ruler
 */
unsigned int
ewe_ruler_step_get(const Eo *obj, Ewe_Ruler_Scale * scale);

/**
 * Set step between marks values in given scale of the ruler.
 *
 * @param obj Ruler object.
 * @param scale Scale. NULL for default scale
 * @param step Step between marks values.
 *
 * @return EINA_TRUE if step was set or EINA_FALSE on failure.
 *
 * @ingroup Ruler
 */
Eina_Bool
ewe_ruler_value_step_set(Evas_Object *obj, Ewe_Ruler_Scale *scale, double step);

/**
 * Get step between marks values in given scale of the ruler
 *
 * @param obj Ruler object.
 * @param scale Scale. NULL for default scale
 *
 * @return value step between marks values.
 *
 * @ingroup Ruler
 */
double
ewe_ruler_value_step_get(const Eo *obj, Ewe_Ruler_Scale *scale);

/**
 * Change the scales "zero pointer" mark position in pixels relative to
 * start  parent's layout
 *
 * @param obj Ruler object.
 * @param scale Scale. NULL for default scale
 * @param pos Position
 *
 * @return EINA_TRUE if pointer was set or EINA_FALSE on failure.
 *
 * @ingroup Ruler
 */
Eina_Bool
ewe_ruler_zero_offset_set(Evas_Object *obj, Ewe_Ruler_Scale * scale, int pos);

/**
 * Get "zero pointer" mark position of given scale
 *
 * @param obj Ruler object.
 * @param scale Scale. NULL for default scale
 *
 * @return value position in pixels "zero pointer" from start parent's layout.
 *
 * @ingroup Ruler
 */
int
ewe_ruler_zero_offset_get(const Eo *obj, Ewe_Ruler_Scale * scale);

/**
 * Set format string for given scale of the ruler.
 *
 * @param obj Ruler object.
 * @param scale Scale. NULL for default scale
 * @param format New format string.
 *
 * @return EINA_TRUE if format string was setted or EINA_FALSE on failure.
 *
 * @ingroup Ruler
 */
Eina_Bool
ewe_ruler_format_set(Evas_Object *obj, Ewe_Ruler_Scale *scale, const char *format);

/**
 * Get format string of given scale
 *
 * @param obj Ruler object.
 * @param scale Scale. NULL for default scale
 *
 * @return format string.
 *
 * @ingroup Ruler
 */
const char *
ewe_ruler_format_get(const Eo *obj, Ewe_Ruler_Scale *scale);

/**
 * Set style for given scale of the ruler.
 *
 * @param obj Ruler object.
 * @param scale Scale. NULL for default scale
 * @param style New style.
 *
 * @return EINA_TRUE if style was setted or EINA_FALSE on failure.
 *
 * @ingroup Ruler
 */
Eina_Bool
ewe_ruler_style_set(Evas_Object *obj, Ewe_Ruler_Scale *scale, const char *style);

/**
 * Get style of given scale
 *
 * @param obj Ruler object.
 * @param scale Scale. NULL for default scale
 *
 * @return style string.
 *
 * @ingroup Ruler
 */
const char *
ewe_ruler_style_get(const Eo *obj, Ewe_Ruler_Scale *scale);

/**
 * Add new scale to the ruler
 *
 * @param obj Ruler object.
 * @param style Style for new scale. NULL for default style
 *
 * @return pointer to new Ewe_Ruler_Scale
 *
 * @ingroup Ruler
 */
Ewe_Ruler_Scale *
ewe_ruler_scale_add(Eo *obj, const char *style);

/**
 * Delete given scale from the ruler
 *
 * @param obj Ruler object.
 * @param scale Scale
 *
 * @return EINA_TRUE on success, EINA_FALSE otherwise
 *
 * @ingroup Ruler
 */
Eina_Bool
ewe_ruler_scale_del(Eo *obj, Ewe_Ruler_Scale *scale);

/**
 * Add new marker to the ruler
 *
 * @param obj Ruler object.
 * @param style Style for new marker. NULL for default style
 *
 * @return pointer to new Ewe_Ruler_Marker
 *
 * @ingroup Ruler
 */
Ewe_Ruler_Marker *
ewe_ruler_marker_add(Eo *obj, const char *style);

/**
 * Delete given marker from the ruler
 *
 * @param obj Ruler object.
 * @param marker Marker
 *
 * @return EINA_TRUE on success, EINA_FALSE otherwise
 *
 * @ingroup Ruler
 */
Eina_Bool
ewe_ruler_marker_del(Eo *obj, Ewe_Ruler_Marker *marker);

/**
 * Set the marker size.
 *
 * @param obj Ruler object.
 * @param marker Marker
 * @param size The new marker size
 *
 * @ingroup Ruler
 */
void
ewe_ruler_marker_size_set(Eo *obj, Ewe_Ruler_Marker *marker, int size);

/**
 * Set a tooltip for given marker.
 *
 * @param obj Ruler object.
 * @param marker Marker
 * @param text The tooltip text
 *
 * @ingroup Ruler
 */
void
ewe_ruler_marker_tooltip_set(Eo *obj, Ewe_Ruler_Marker *marker, const char *text);

/**
 * Get visiblility state of given scale
 *
 * @param obj Ruler object.
 * @param scale Scale. NULL for default scale
 *
 * @return EINA_TRUE on success, EINA_FALSE otherwise
 *
 * @ingroup Ruler
 */
Eina_Bool
ewe_ruler_scale_visible_get(const Eo *obj, Ewe_Ruler_Scale *scale);

/**
 * Set visiblility state of given scale
 *
 * @param obj Ruler object.
 * @param scale Scale. NULL for default scale
 * @param visible New visibility state for scale
 *
 * @return EINA_TRUE on success, EINA_FALSE otherwise
 *
 * @ingroup Ruler
 */
Eina_Bool
ewe_ruler_scale_visible_set(Eo *obj, Ewe_Ruler_Scale *scale, Eina_Bool visible);

/**
 * Get visiblility state of middle mark in given scale
 *
 * @param obj Ruler object.
 * @param scale Scale. NULL for default scale
 *
 * @return EINA_TRUE if mark is visible, EINA_FALSE otherwise
 *
 * @ingroup Ruler
 */
Eina_Bool
ewe_ruler_scale_middle_mark_get(const Eo *obj, Ewe_Ruler_Scale *scale);

/**
 * Set visiblility state of middle mark in given scale
 *
 * @param obj Ruler object.
 * @param scale Scale. NULL for default scale
 * @param middle_mark New visibility state for middle mark in given scale
 *
 * @return EINA_TRUE on success, EINA_FALSE otherwise
 *
 * @ingroup Ruler
 */
Eina_Bool
ewe_ruler_scale_middle_mark_set(Eo *obj, Ewe_Ruler_Scale *scale, Eina_Bool middle_mark);

/**
 * Set marker to folow specified value on given scale.
 *
 * @param obj Ruler object.
 * @param marker Marker.
 * @param scale Scale. NULL for default scale
 * @param position New relative position.
 *
 * @return EINA_TRUE if position was set or EINA_FALSE on failure.
 *
 * @ingroup Ruler
 */
Eina_Bool
ewe_ruler_marker_relative_set(Evas_Object *obj, Ewe_Ruler_Marker *marker, Ewe_Ruler_Scale *scale, double position);

/**
 * Get marker position relative to given scale
 *
 * @param obj Ruler object.
 * @param marker Marker.
 * @param scale Scale. NULL for default scale
 *
 * @return relative position of marker
 *
 * @ingroup Ruler
 */
double
ewe_ruler_marker_relative_get(const Eo *obj, Ewe_Ruler_Marker *marker, Ewe_Ruler_Scale *scale);

/**
 * Set marker position.
 *
 * @param obj Ruler object.
 * @param marker Marker.
 * @param position New position.
 *
 * @return EINA_TRUE if position was set or EINA_FALSE on failure.
 *
 * @ingroup Ruler
 */
Eina_Bool
ewe_ruler_marker_absolute_set(Evas_Object *obj, Ewe_Ruler_Marker *marker, int position);

/**
 * Get marker position
 *
 * @param obj Ruler object.
 * @param marker Marker.
 *
 * @return position of marker
 *
 * @ingroup Ruler
 */
int
ewe_ruler_marker_absolute_get(const Eo *obj, Ewe_Ruler_Marker *marker);

/**
 * Get visiblility state of given marker
 *
 * @param obj Ruler object.
 * @param marker Marker. NULL for default marker
 *
 * @return EINA_TRUE on success, EINA_FALSE otherwise
 *
 * @ingroup Ruler
 */
Eina_Bool
ewe_ruler_marker_visible_get(const Eo *obj, Ewe_Ruler_Marker *marker);

/**
 * Set visiblility state of given marker
 *
 * @param obj Ruler object.
 * @param marker Marker. NULL for default marker
 * @param visible New visibility state for marker
 *
 * @return EINA_TRUE on success, EINA_FALSE otherwise
 *
 * @ingroup Ruler
 */
Eina_Bool
ewe_ruler_marker_visible_set(Eo *obj, Ewe_Ruler_Marker *marker, Eina_Bool visible);

/**
 * Set style for given marker.
 *
 * @param obj Ruler object.
 * @param marker Marker.
 * @param style New style.
 *
 * @return EINA_TRUE if style was setted or EINA_FALSE on failure.
 *
 * @ingroup Ruler
 */
Eina_Bool
ewe_ruler_marker_style_set(Evas_Object *obj, Ewe_Ruler_Marker *marker, const char *style);

/**
 * Get style of given marker
 *
 * @param obj Ruler object.
 * @param marker Marker.
 *
 * @return style string.
 *
 * @ingroup Ruler
 */
const char *
ewe_ruler_marker_style_get(const Eo *obj, Ewe_Ruler_Marker *marker);

#endif /* EWE_RULER_LEGACY_H */

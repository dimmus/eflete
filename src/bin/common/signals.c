/*
 * Edje Theme Editor
 * Copyright (C) 2013-2016 Samsung Electronics.
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

#include "history.h"
#include "signals.h"

const Signals signals = {
     .shortcut = {
          .del                      = "signal.shortcut.del",
          .help                     = "signal.shortcut.help",
          .quit                     = "signal.shortcut.quit",
          .save                     = "signal.shortcut.save",
          .add = {
               .data_item           = "signal.shortcut.add.data_item",
               .group               = "signal.shortcut.add.group",
               .item                = "signal.shortcut.add.item",
               .part                = "signal.shortcut.add.part",
               .program             = "signal.shortcut.add.program",
               .state               = "signal.shortcut.add.state",
          },
          .history = {
               .redo                = "signal.shortcut.history.redo",
               .undo                = "signal.shortcut.history.undo",
          },
          .manager = {
               .color_class         = "signal.shortcut.manager.color_class",
               .image               = "signal.shortcut.manager.image",
               .sound               = "signal.shortcut.manager.sound",
               .style               = "signal.shortcut.manager.style",
          },
          .popup = {
               .cancel              = "signal.shortcut.popup.cancel",
               .done                = "signal.shortcut.popup.done",
          },
          .tab = {
               .close               = "signal.shortcut.tab.close",
               .next                = "signal.shortcut.tab.next",
               .prev                = "signal.shortcut.tab.prev",
               .num                 = "signal.shortcut.tab.num",
          },
          .workspace = {
               .fill                = "signal.shortcut.workspace.fill",
               .mode = {
                    .code           = "signal.shortcut.workspace.mode.code",
                    .demo           = "signal.shortcut.workspace.mode.demo",
                    .normal         = "signal.shortcut.workspace.mode.normal",
               },
               .show_hide = {
                    .object_area    = "signal.shortcut.workspace.show_hide.object_area",
                    .part           = "signal.shortcut.workspace.show_hide.part",
                    .all_parts      = "signal.shortcut.workspace.show_hide.all_parts",
                    .rulers         = "signal.shortcut.workspace.show_hide.rulers",
               },
               .select = {
                    .part_next      = "signal.shortcut.workspace.select.part_next",
                    .part_prev      = "signal.shortcut.workspace.select.part_prev",
                    .state_next     = "signal.shortcut.workspace.select.state_next",
               },
               .zoom = {
                    .in             = "signal.shortcut.workspace.zoom.in",
                    .out            = "signal.shortcut.workspace.zoom.out",
                    .reset          = "signal.shortcut.workspace.zoom.reset",
                    .fit            = "signal.shortcut.workspace.zoom.fit",
               },
          },
     },

     .elm = {
          .spinner = {
               .changed             = "changed",
               .changed_user        = "changed",
               .drag_start          = "spinner,drag,start",
               .drag_stop           = "spinner,drag,stop",
          },
     },
};

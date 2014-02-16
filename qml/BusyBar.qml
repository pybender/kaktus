/*
  Copyright (C) 2014 Michal Kosciesza <michal@mkiol.net>

  This file is part of Kaktus.

  Kaktus is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Kaktus is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Kaktus.  If not, see <http://www.gnu.org/licenses/>.
*/

import QtQuick 2.0
import Sailfish.Silica 1.0



Rectangle {
    id: root

    property string text
    property bool cancelable: false
    property bool open: false

    signal closeClicked

    height: visible ? Theme.itemSizeSmall : 0
    width: parent.width

    anchors.bottom: parent.bottom
    anchors.left: parent.left

    /*gradient: Gradient {
        GradientStop { position: -0.5; color: "transparent" }
        GradientStop { position: 1.5; color: Theme.highlightBackgroundColor}
    }*/

    color: Theme.highlightBackgroundColor
    enabled: opacity > 0.0

    opacity: root.open ? 1.0 : 0.0
    Behavior on opacity { FadeAnimation {} }

    function show(text, cancelable) {
        root.text = text;
        root.cancelable = cancelable;
        root.open = true;
    }

    function hide() {
        root.open = false;
    }

    Row {
        spacing: Theme.paddingMedium
        anchors{
            left: parent.left; leftMargin: Theme.paddingMedium
            right: parent.right; rightMargin: Theme.paddingMedium
            verticalCenter: parent.verticalCenter
        }

        Image {
            id: icon
            anchors.verticalCenter: parent.verticalCenter
            //source: "image://theme/icon-m-sync" + "?" + Theme.highlightDimmerColor
            source: "image://theme/graphic-busyindicator-medium" + "?" + Theme.highlightDimmerColor

            RotationAnimation on rotation {
                loops: Animation.Infinite
                from: 0
                to: 360
                duration: 1000
                running: root.open
            }
        }

        Label {
            id: titleBar
            font.pixelSize: Theme.fontSizeSmall
            font.family: Theme.fontFamily
            text: root.text
            width: parent.width - icon.width * 2 - parent.spacing *2
            height: icon.height
            anchors.verticalCenter: parent.verticalCenter
            color: Theme.highlightDimmerColor
            verticalAlignment: Text.AlignVCenter
        }

        IconButton {
            anchors.verticalCenter: parent.verticalCenter
            icon.source: "image://theme/icon-m-close" + "?" + Theme.highlightDimmerColor
            onClicked: root.closeClicked()
            visible: root.cancelable
        }

    }
}
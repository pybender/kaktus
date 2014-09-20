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


Item {
    id: root

    property bool open: false
    property bool openable: true
    property int showTime: 6000

    property real barShowMoveWidth: 20
    property Flickable flick: null

    property bool isPortrait: app.orientation==Orientation.Portrait

    height: Theme.itemSizeMedium
    width: parent.width

    function show() {
        if (openable && pageStack.currentPage.showBar) {
            if (!open)
                root.open = true;
            timer.restart();
        }
    }

    function showAndEnable() {
        openable = true
        show();
    }

    function hide() {
        if (open) {
            root.open = false;
            timer.stop();
        }
    }

    function hideAndDisable() {
        hide();
        openable = false;
    }

    Item {
        id: bar
        anchors.fill: parent

        opacity: root.open ? 1.0 : 0.0
        visible: opacity > 0.0
        Behavior on opacity { FadeAnimation {duration: 300} }

        Rectangle {
            id: leftBg

            Rectangle {
                color: Theme.highlightBackgroundColor
                anchors.left: parent.left;
                height: parent.height
                width: parent.width>parent.radius ? parent.radius : 0
            }

            anchors.left: parent.left;
            width: settings.viewMode==0 ? 0 :
                   settings.viewMode==1 ? vm1b.x-Theme.paddingSmall :
                   settings.viewMode==3 ? vm3b.x-Theme.paddingSmall :
                   settings.viewMode==4 ? vm4b.x-Theme.paddingSmall :
                   settings.viewMode==5 ? vm5b.x-Theme.paddingSmall :
                   0
            height: 2*parent.height
            radius: 10
            color: Theme.highlightBackgroundColor
            Behavior on width { NumberAnimation { duration: 200;easing.type: Easing.OutQuad } }
        }

        Rectangle {
            id: rightBg

            Rectangle {
                color: Theme.highlightBackgroundColor
                anchors.right: parent.right;
                height: parent.height
                width: parent.radius
            }

            anchors.right: parent.right
            width: settings.viewMode==0 ? parent.width-(vm0b.x+vm0b.width+Theme.paddingSmall) :
                   settings.viewMode==1 ? parent.width-(vm1b.x+vm1b.width+Theme.paddingSmall) :
                   settings.viewMode==3 ? parent.width-(vm3b.x+vm3b.width+Theme.paddingSmall) :
                   settings.viewMode==4 ? parent.width-(vm4b.x+vm4b.width+Theme.paddingSmall) :
                   settings.viewMode==5 ? parent.width-(vm5b.x+vm5b.width+Theme.paddingSmall) :
                   parent.width-(vm0b.x+vm0b.width+Theme.paddingSmall)

            height: 2*parent.height
            radius: 10
            color: Theme.highlightBackgroundColor
            Behavior on width { NumberAnimation { duration: 200;easing.type: Easing.OutQuad } }
        }

        MouseArea {
            enabled: bar.visible
            anchors.fill: parent
            onClicked: root.hide()
        }

        IconButton {
            id: vm0b
            x: 0*(width+Theme.paddingMedium)
            anchors.verticalCenter: parent.verticalCenter
            icon.source: "image://icons/vm0?"+Theme.highlightDimmerColor
            highlighted: settings.viewMode==0
            onClicked: {
                settings.viewMode = 0;
                show();
            }
        }

        IconButton {
            id: vm1b
            x: 1*(width+Theme.paddingMedium)
            anchors.verticalCenter: parent.verticalCenter
            icon.source: "image://icons/vm1?"+Theme.highlightDimmerColor
            highlighted: settings.viewMode==1
            onClicked: {
                settings.viewMode = 1;
                show();
            }
        }

        IconButton {
            id: vm3b
            x: 2*(width+Theme.paddingMedium)
            anchors.verticalCenter: parent.verticalCenter
            icon.source: "image://icons/vm3?"+Theme.highlightDimmerColor
            highlighted: settings.viewMode==3
            onClicked: {
                settings.viewMode = 3;
                show();
            }
        }

        IconButton {
            id: vm4b
            x: 3*(width+Theme.paddingMedium)
            anchors.verticalCenter: parent.verticalCenter
            icon.source: "image://icons/vm4?"+Theme.highlightDimmerColor
            highlighted: settings.viewMode==4
            onClicked: {
                settings.viewMode = 4;
                show();
            }
        }

        IconButton {
            id: vm5b
            x: 4*(width+Theme.paddingMedium)
            anchors.verticalCenter: parent.verticalCenter
            icon.source: "image://icons/vm5?"+(root.transparent ? Theme.primaryColor : Theme.highlightDimmerColor)
            highlighted: settings.viewMode==5
            onClicked: {
                settings.viewMode = 5;
                show();
            }
        }

        IconButton {
            id: offline
            anchors.right: parent.right; anchors.rightMargin: Theme.paddingSmall
            anchors.verticalCenter: parent.verticalCenter
            icon.source: settings.offlineMode ? "image://theme/icon-m-wlan-no-signal?"+(root.transparent ? Theme.primaryColor : Theme.highlightDimmerColor)
                                              : "image://theme/icon-m-wlan-4?"+(root.transparent ? Theme.primaryColor : Theme.highlightDimmerColor)
            onClicked: {
                show();
                if (settings.offlineMode) {
                    if (dm.online)
                        settings.offlineMode = false;
                    else
                        notification.show(qsTr("Cannot switch to Online mode\nNetwork connection is unavailable"));
                } else {
                    settings.offlineMode = true;
                }
            }
        }
    }

    MouseArea {
        enabled: !bar.visible
        anchors.bottom: parent.bottom; anchors.left: parent.left; anchors.right: parent.right
        height: root.height/2
        onClicked: root.show();
    }

    Timer {
        id: timer
        interval: root.showTime
        onTriggered: hide();
    }

    QtObject {
        id: m
        property real initialContentY: 0.0
        property real lastContentY: 0.0
        property int vector: 0
    }

    Connections {
        target: flick

        onMovementStarted: {
            m.vector = 0;
            m.lastContentY = 0.0;
            m.initialContentY=flick.contentY;
        }

        onContentYChanged: {
            if (flick.moving) {
                var dInit = flick.contentY-m.initialContentY;
                var dLast = flick.contentY-m.lastContentY;
                var lastV = m.vector;
                if (dInit<-barShowMoveWidth)
                    root.show();
                if (dInit>barShowMoveWidth)
                    root.hide();
                if (dLast>barShowMoveWidth)
                    root.hide();
                if (m.lastContentY!=0) {
                    if (dLast<0)
                        m.vector = -1;
                    if (dLast>0)
                        m.vector = 1;
                    if (dLast==0)
                        m.vector = 0;
                }
                if (lastV==-1 && m.vector==1)
                    m.initialContentY=flick.contentY;
                if (lastV==1 && m.vector==-1)
                    m.initialContentY=flick.contentY;
                m.lastContentY = flick.contentY;
            }
        }
    }

}

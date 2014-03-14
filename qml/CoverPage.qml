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



CoverBackground {
    id: root

    property bool busy: false
    property real progress: 0.0

    Rectangle {
        id: progressRect
        height: parent.height
        anchors.right: parent.right
        width: parent.width - (root.progress * parent.width)
        color: Theme.highlightDimmerColor
        //color: Theme.highlightBackgroundColor
        opacity: 0.5
        visible: root.busy

        Behavior on width {
            enabled: root.busy
            SmoothedAnimation {
                velocity: 480; duration: 200
            }
        }
    }

    Column {
        anchors.centerIn: parent
        visible: !root.busy
        spacing: Theme.paddingMedium

        Image {
            source: "icon-small.png"
        }
        Label {
            font.pixelSize: Theme.fontSizeMedium
            font.family: Theme.fontFamilyHeading
            anchors.horizontalCenter: parent.horizontalCenter
            text: APP_NAME
        }
    }

    Column {
        visible: root.busy
        anchors.left: parent.left; anchors.right: parent.right
        anchors.margins: Theme.paddingSmall
        anchors.verticalCenter: parent.verticalCenter
        spacing: Theme.paddingMedium

        /*Image {
            anchors.horizontalCenter: parent.horizontalCenter
            source: "image://theme/icon-l-download"
            visible: root.busy
        }*/

        Label {
            id: progressLabel
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: Theme.fontSizeLarge
            font.family: Theme.fontFamilyHeading
            color: Theme.highlightColor
        }

        Label {
            id: label
            anchors.left: parent.left; anchors.right: parent.right
            font.pixelSize: Theme.fontSizeMedium
            font.family: Theme.fontFamilyHeading
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.Wrap
        }

    }

    CoverActionList {
        CoverAction {
            id: action
            iconSource: root.busy ? "image://theme/icon-cover-cancel" : "image://theme/icon-cover-sync"
            onTriggered: {
                if (root.busy) {
                    dm.cancel();
                    fetcher.cancel();
                } else {
                    dm.cancel();
                    fetcher.cancel();
                    fetcher.update();
                }
            }
        }
    }

    Connections {
        target: fetcher
        onBusy: {
            label.text = qsTr("Syncing");
            progressLabel.text = "";
            root.progress = 0.0;
            root.busy = true
        }
        onError: {
            root.busy = false;
            //label.text = qsTr("Error!");
        }
        onReady: {
            if (!dm.isBusy())
                root.busy = false;
        }
        onProgress: {
            label.text = qsTr("Syncing");
            progressLabel.text = Math.floor((current/total)*100)+"%";
            root.progress = current / total;
        }

    }

    Connections {
        target: dm
        onCanceled: {
            if (!fetcher.isBusy())
                root.busy = false;
        }
        onBusy: root.busy = true
        onReady: {
            if (!fetcher.isBusy()) {
                label.text = qsTr("Caching");
                root.busy = false;
            }
        }
        onProgress: {
            if (!fetcher.isBusy()) {
                label.text = qsTr("Caching");
                progressLabel.text = remaining;
            }
        }
    }
}



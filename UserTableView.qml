import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

import UserTableModel 0.1

Item {

    anchors.fill: parent

    property bool haveHeader: true
    property color headerDelegateColor: "#333333"
    property color headerLabelColor: "#aaaaaa"
    property real headerLabelPixelSize: 15
    property real headerLabelPadding: 10
    property int headerLabelHorizontalAlignment: Text.AlignHCenter
    property int headerLabelVerticalAlignment: Text.AlignVCenter

    function funcRowHeight(row) {
        var that = this.parent;
        return that.model ? that.height / (that.rows + 1) : 0;
    }
    function funcColumnWidth(column) {
        var that = this.parent;
        return that.model ? that.width / that.columns : 0;
    }

    property real delegateBorderWidth: 1
    property color delegateBorderColor: "gray"

    property real cellTextPixelSize: 16
    property color cellTextColor: "black"

    TableView {
        id: tableView

        anchors.fill: parent

        topMargin: columnsHeader.implicitHeight

        contentWidth: parent.implicitWidth

        contentHeight: parent.implicitHeight + topMargin

        model: usertablemodel

        clip: true

        rowHeightProvider: funcRowHeight
        columnWidthProvider: funcColumnWidth

        onWidthChanged: forceLayout()

        onHeightChanged: forceLayout()

        Row {
            id: columnsHeader
            y: tableView.contentY
            z: 2

            enabled: haveHeader

            visible: tableView.rows > 0

            Repeater {
                model: tableView.columns > 0 ? tableView.columns : 1

                Rectangle {
                    width: tableView.columnWidthProvider(modelData)
                    height: tableView.rowHeightProvider(modelData)
                    color: headerDelegateColor
                    clip: true

                    Label {
                        width: parent.width
                        height: parent.height
                        text: usertablemodel ? usertablemodel.headerData(modelData, Qt.Horizontal) : ""
                        color: headerLabelColor
                        font.pixelSize: headerLabelPixelSize
                        padding: headerLabelPadding
                        clip: true
                        horizontalAlignment: headerLabelHorizontalAlignment
                        verticalAlignment: headerLabelVerticalAlignment
                    }
                }
            }
        }

        delegate: Rectangle {
            implicitWidth: tableView.columnWidthProvider(column)
            implicitHeight: tableView.rowHeightProvider(row)
            border.width: delegateBorderWidth
            border.color: delegateBorderColor

            Text {
                id: cell
                text: display
                color: cellTextColor
                anchors.centerIn: parent
                font.pixelSize: cellTextPixelSize
            }
        }
    }

    Placeholder { visibleCondition: tableView.rows === 0 }
}

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

Item {
    id: placeholder

    anchors.centerIn: parent

    property bool visibleCondition

    property url imageSource: "qrc:/empty-table-placeholder.png"

    property string underText: "Пока ничего нет."

    property bool underTextIsBold: true

    ColumnLayout {

        spacing: 20

        visible: visibleCondition

        anchors.centerIn: parent

        Image {
            id: placeholder_image
            Layout.alignment: Qt.AlignHCenter
            source: imageSource
        }

        Label {
            id: placeholder_label
            text: underText
            font.bold: underTextIsBold
            Component.onCompleted: font.pointSize += 2
        }
    }
}

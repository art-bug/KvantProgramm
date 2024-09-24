import QtQuick 2.15
import QtQuick.Dialogs 1.2

MessageDialog {
    property string errorWarningReasonText: reasonText

    property int messageIcon: StandardIcon.Warning

    title: "Что-то пошло не так..."
    text: errorWarningReasonText
    icon: messageIcon
    modality: Qt.ApplicationModal
    Component.onCompleted: visible = true
    onAccepted: close()
}

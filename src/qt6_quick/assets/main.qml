import QtQuick
import QtQuick.Window
import QtQuick.Controls

Window {
    width: 960
    height: 540
    visible: true
    title: qsTr("vcpkg_demo_qt6_quick")

    Image {
        anchors.fill: parent
        fillMode: Image.PreserveAspectCrop
        source: "qrc:/test.jpg"
    }
}

import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    width: 960
    height: 540
    visible: true
    title: qsTr("vcpkg_demo_qt5_quick")

    Image {
        anchors.fill: parent
        source: "qrc:/test.jpg"
    }
}

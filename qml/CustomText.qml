import QtQuick

Text {
    id: parent

    required property string customText

    text: parent.customText
    color: "white"

    font.family: fontLoader.font.family
    font.weight: fontLoader.font.weight
    font.styleName: fontLoader.font.styleName
    font.pixelSize: 24
}

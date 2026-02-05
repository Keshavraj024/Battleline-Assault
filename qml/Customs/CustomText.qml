import QtQuick

Text {
    id: parent

    required property string customText
    required property FontLoader fontName

    text: parent.customText
    color: "white"

    font.family: fontName.font.family
    font.weight: fontName.font.weight
    font.styleName: fontName.font.styleName
    font.pixelSize: 24
}

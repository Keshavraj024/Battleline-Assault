import QtQuick
import QtQuick.Controls


Button {
    id: mainMenuButton

    property alias labelText: buttonText.text
    property alias labelSize: buttonText.font.pixelSize
    property color hoverColor: "white"
    property color normalColor: "#aaaaaa"


    signal actionTriggered()

    hoverEnabled: true
    background: null

    contentItem : Text {
        id: buttonText

        font.family: menuFontLoader.font.family
        font.weight: menuFontLoader.font.weight
        font.styleName: menuFontLoader.font.styleName
        font.pixelSize: 24
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

        style: Text.Outline
        styleColor: "#44000000"

        antialiasing: true

        scale: mainMenuButton.hovered ? 1.1 : 1.0
        color: mainMenuButton.hovered ? mainMenuButton.hoverColor : mainMenuButton.normalColor
        Behavior on scale { NumberAnimation { duration: 150; easing.type: Easing.OutBack } }
        Behavior on color { NumberAnimation { duration: 150}}

    }

    onClicked: function() {
        mainMenuButton.actionTriggered()
    }

    onHoveredChanged: function() {
        if(hovered && GameController)
            GameController.playClickSound()
    }
}

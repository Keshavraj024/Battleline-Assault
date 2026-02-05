import QtQuick
import QtQuick.Layouts
import "../Customs"

ColumnLayout {
    id: hudRoot

    property int score: 0
    property int highestScore: 0
    property int gameLevel: 0
    property bool isGameRunning: false

    spacing: 10
    visible: isGameRunning

    FontLoader {
        id: gameUHDFont
        source : "../Assets/Fonts/Doto-ExtraBold.ttf"
    }


    CustomText {
        id: scoreBoard
        fontName: gameUHDFont
        customText: "Score : " + hudRoot.score
        Layout.leftMargin: 5
    }

    CustomText {
        id: highestScoreBoard
        fontName: gameUHDFont
        customText: "Highest Score : " + hudRoot.highestScore
        Layout.leftMargin: 5
    }

    CustomText {
        id: levelBoard
        fontName: gameUHDFont
        customText: "Level : " + hudRoot.gameLevel
        Layout.leftMargin: 5
    }
}

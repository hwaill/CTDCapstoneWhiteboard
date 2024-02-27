var todoCheckBox = document.getElementById("Todo-Check");
var weeklyCalCheckBox = document.getElementById("WeeklyCal-Check");
var quotesCheckBox = document.getElementById("Quotes-Check");
var weatherCheckBox = document.getElementById("Weather-Check");
var gameCheckBox = document.getElementById("Game-Check");
var moodCheckBox = document.getElementById("Mood-Check");
var themeCheckBox = document.getElementById("Theme-Check");

var theme = document.getElementById("themes-list");
var spaceCheckBox = document.getElementById("spaceTheme-Check");
var plantsCheckBox = document.getElementById("plantTheme-Check");
var heartsCheckBox = document.getElementById("heartTheme-Check");
var swirlsCheckBox = document.getElementById("swirlTheme-Check");
var blankCheckBox = document.getElementById("noneTheme-Check");

var featureSelectedString;
var themeSelectedString;

// 1234567
// order: Todo[1], WeeklyCalendar[2], Quotes[3], Weather[4], Game[5], Mood[6], Theme[7]

function featureSelected() {
  
    var todoValue = todoCheckBox.checked ? "1" : "0";
    var weeklyCalValue = weeklyCalCheckBox.checked ? "1" : "0";
    var quotesValue = quotesCheckBox.checked ? "1" : "0";
    var weatherValue = weatherCheckBox.checked ? "1" : "0";
    var gameValue = gameCheckBox.checked ? "1" : "0";
    var moodValue = moodCheckBox.checked ? "1" : "0";
    var themeValue = themeCheckBox.checked ? "1" : "0";

    featureSelectedString = todoValue + weeklyCalValue + quotesValue + weatherValue + gameValue + moodValue + themeValue;

    console.log(featureSelectedString);
}

function themeSelected(){
    alert(weatherValue.value);
}
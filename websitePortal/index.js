var todoCheckBox = document.getElementById("Todo-Check");
var weeklyCalCheckBox = document.getElementById("WeeklyCal-Check");
var quotesCheckBox = document.getElementById("Quotes-Check");
var weatherCheckBox = document.getElementById("Weather-Check");
var gameCheckBox = document.getElementById("Game-Check");
var moodCheckBox = document.getElementById("Mood-Check");
var themeCheckBox = document.getElementById("Theme-Check");

var featureSelectedString;

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

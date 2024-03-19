// Your JavaScript code here
// Make sure to access the elements after the DOM is fully loaded
//var navbar = document.getElementById("navbar");
var mainLogo = document.getElementById("mainLogo");
var needsBluetoothScreen = document.getElementById("needsBluetoothConnection");

// var dotContainer = document.getElementById("dot-container");

var featuresPage = document.getElementById("featuresSection");
var boardDisplayPage = document.getElementById("boardDisplayPage");
var profilePage = document.getElementById("profilePage");

var todoPage = document.getElementById("todoPage");
var eventsPage = document.getElementById("eventsPage");
var themePage = document.getElementById("themePage");
var weatherPage = document.getElementById("weatherPage");
var ticTacToePage = document.getElementById("ticTacToePage");
var moodTrackerPage = document.getElementById("moodTrackerPage");
var dailyQuotePage = document.getElementById("dailyQuotePage");

var todoCheckBox = document.getElementById("Todo-Check");
var weeklyCalCheckBox = document.getElementById("WeeklyCal-Check");
var quotesCheckBox = document.getElementById("Quotes-Check");
var weatherCheckBox = document.getElementById("Weather-Check");
var gameCheckBox = document.getElementById("Game-Check");
var moodCheckBox = document.getElementById("Mood-Check");
var themeCheckBox = document.getElementById("Theme-Check");

var spaceCheckBox = document.getElementById("spaceTheme-Check");
var plantsCheckBox = document.getElementById("plantTheme-Check");
var heartsCheckBox = document.getElementById("heartTheme-Check");
var swirlsCheckBox = document.getElementById("swirlTheme-Check");
var blankCheckBox = document.getElementById("noneTheme-Check");


var display_connectResultMessage = document.getElementById("connectionResultMessage");
var display_firstName = document.getElementById("firstName");
var display_lastName = document.getElementById("lastName");

var eventDateTitle = document.getElementById("eventDate-label");
var eventDate = document.getElementById("eventDate");
var eventName = document.getElementById("eventName");

eventDateTitle.style.display = "none";
eventDate.style.display = "none";

var featureSelectedString;
var themeSelectedString;

//Variables that hold configuration values:
var config_firstName;
var config_lastName;
var config_theme;
var config_numFeatures;
var config_features;
var config_wifiSSID;
var config_wifiPass;
var config_timeZone;
var config_zipCode;
var config_numMoodQuestions;
var config_moodQuestions;
var config_numMorningToDos;
var config_numDaytimeToDos;
var config_numEveningToDos;
const config_morningToDos = [];
const config_daytimeToDos = [];
const config_eveningToDos = [];

goLanding();

//BLE variables and what not...
let dataCharacteristic, requestNameCharacteristic, indexCharacteristic, portalSideRequestCharacteristic, portalHasUpdateCharacteristic, successResponseCharacteristic;

const serviceUUID = "722cf000-6c3d-48ac-8180-64551d967680";

const dataCharacteristic1UUID = "722cf001-6c3d-48ac-8180-64551d967680";
const requestNameUUID = "722cf002-6c3d-48ac-8180-64551d967680";
const indexCharacteristicUUID = "722cf003-6c3d-48ac-8180-64551d967680";
const portalSideRequestUUID = "722cf004-6c3d-48ac-8180-64551d967680";
const portalHasUpdateUUID = "722cf005-6c3d-48ac-8180-64551d967680";
const successResponseUUID = "722cf006-6c3d-48ac-8180-64551d967680";

let decoder = new TextDecoder("utf-8");
let encoder = new TextEncoder("utf-8");

let trueBuffer = new Uint8Array(new ArrayBuffer(1));
trueBuffer[0] = true;

var result;

async function connectToBluetooth() {
    const device = await navigator.bluetooth.requestDevice({filters: [{services: [serviceUUID]}]});
    display_connectResultMessage.innerText = "connecting...";
    const server = await device.gatt.connect();
    const service = await server.getPrimaryService(serviceUUID);

    console.log("test");
    dataCharacteristic = await service.getCharacteristic(dataCharacteristic1UUID);
    requestNameCharacteristic = await service.getCharacteristic(requestNameUUID);
    indexCharacteristic = await service.getCharacteristic(indexCharacteristicUUID);
    portalSideRequestCharacteristic = await service.getCharacteristic(portalSideRequestUUID);
    portalHasUpdateCharacteristic = await service.getCharacteristic(portalHasUpdateUUID);
    successResponseCharacteristic = await service.getCharacteristic(successResponseUUID);
    
    display_connectResultMessage.innerText = "loading first name...";
    await requestNameCharacteristic.writeValueWithResponse(encoder.encode("firstName\0"));
    await portalSideRequestCharacteristic.writeValue(trueBuffer);
    result = await dataCharacteristic.readValue();
    config_firstName = decoder.decode(result);
    console.log("Last name: " + config_firstName);

    display_connectResultMessage.innerText = "loading last name...";
    await requestNameCharacteristic.writeValueWithResponse(encoder.encode("lastName\0"));
    await portalSideRequestCharacteristic.writeValue(trueBuffer);
    result = await dataCharacteristic.readValue();
    config_lastName = decoder.decode(result);
    console.log("Last name: " + config_lastName);

    display_connectResultMessage.innerText = "loading theme preference...";
    await requestNameCharacteristic.writeValueWithResponse(encoder.encode("theme\0"));
    await portalSideRequestCharacteristic.writeValue(trueBuffer);
    result = await dataCharacteristic.readValue();
    config_theme = decoder.decode(result);
    console.log("Theme: " + config_theme);

    display_connectResultMessage.innerText = "loading enabled features...";
    await requestNameCharacteristic.writeValueWithResponse(encoder.encode("numFeatures\0"));
    await portalSideRequestCharacteristic.writeValue(trueBuffer);
    result = await dataCharacteristic.readValue();
    config_numFeatures = decoder.decode(result);
    console.log("Num features: " + config_numFeatures);

    await requestNameCharacteristic.writeValueWithResponse(encoder.encode("features\0"));
    await portalSideRequestCharacteristic.writeValue(trueBuffer);
    result = await dataCharacteristic.readValue();
    config_features = decoder.decode(result);
    console.log("Features: " + config_features);

    display_connectResultMessage.innerText = "loading mood tracking preferences...";
    await requestNameCharacteristic.writeValueWithResponse(encoder.encode("numMoodQuestions\0"));
    await portalSideRequestCharacteristic.writeValue(trueBuffer);
    result = await dataCharacteristic.readValue();
    config_numMoodQuestions = decoder.decode(result);
    console.log("Num mood questions: " + config_numMoodQuestions);

    await requestNameCharacteristic.writeValueWithResponse(encoder.encode("moodQuestions\0"));
    await portalSideRequestCharacteristic.writeValue(trueBuffer);
    result = await dataCharacteristic.readValue();
    config_moodQuestions = decoder.decode(result);
    console.log("Mood questions: " + config_moodQuestions);

    display_connectResultMessage.innerText = "loading wifi credentials...";
    await requestNameCharacteristic.writeValueWithResponse(encoder.encode("wifiSSID\0"));
    await portalSideRequestCharacteristic.writeValue(trueBuffer);
    result = await dataCharacteristic.readValue();
    config_wifiSSID = decoder.decode(result);
    console.log("Wifi SSID: " + config_wifiSSID);

    await requestNameCharacteristic.writeValueWithResponse(encoder.encode("wifiPass\0"));
    await portalSideRequestCharacteristic.writeValue(trueBuffer);
    result = await dataCharacteristic.readValue();
    config_wifiPass = decoder.decode(result);
    console.log("Wifi Pass: " + config_wifiPass);

    display_connectResultMessage.innerText = "loading time zone...";
    await requestNameCharacteristic.writeValueWithResponse(encoder.encode("timeZone\0"));
    await portalSideRequestCharacteristic.writeValue(trueBuffer);
    result = await dataCharacteristic.readValue();
    config_timeZone = decoder.decode(result);
    console.log("Time Zone: " + config_timeZone);

    display_connectResultMessage.innerText = "loading zip code...";
    await requestNameCharacteristic.writeValueWithResponse(encoder.encode("zipCode\0"));
    await portalSideRequestCharacteristic.writeValue(trueBuffer);
    result = await dataCharacteristic.readValue();
    config_zipCode = decoder.decode(result);
    console.log("Zip Code: " + config_zipCode);

    display_connectResultMessage.innerText = "loading morning to-do list...";
    await requestNameCharacteristic.writeValueWithResponse(encoder.encode("numMorningToDos\0"));
    await portalSideRequestCharacteristic.writeValue(trueBuffer);
    result = await dataCharacteristic.readValue();
    config_numMorningToDos = decoder.decode(result);
    console.log("Num Morning To-Dos: " + config_numMorningToDos);

    await requestNameCharacteristic.writeValueWithResponse(encoder.encode("morningToDo\0"));
    for(var i = 0; i < config_numMorningToDos; i++) {
        const uint8 = new Uint8Array(1);
        uint8[0] = i;
        await indexCharacteristic.writeValue(uint8);
        await portalSideRequestCharacteristic.writeValue(trueBuffer);
        result = await dataCharacteristic.readValue();
        config_morningToDos.push(decoder.decode(result));
    }
    console.log("Morning To-Dos: " + config_morningToDos);

    display_connectResultMessage.innerText = "loading daytime to-do list...";
    await requestNameCharacteristic.writeValueWithResponse(encoder.encode("numDaytimeToDos\0"));
    await portalSideRequestCharacteristic.writeValue(trueBuffer);
    result = await dataCharacteristic.readValue();
    config_numDaytimeToDos = decoder.decode(result);
    console.log("Num Daytime To-Dos: " + config_numDaytimeToDos);

    await requestNameCharacteristic.writeValueWithResponse(encoder.encode("daytimeToDo\0"));
    for(var i = 0; i < config_numDaytimeToDos; i++) {
        const uint8 = new Uint8Array(1);
        uint8[0] = i;
        await indexCharacteristic.writeValue(uint8);
        await portalSideRequestCharacteristic.writeValue(trueBuffer);
        result = await dataCharacteristic.readValue();
        config_daytimeToDos.push(decoder.decode(result));
    }
    console.log("Daytime To-Dos: " + config_daytimeToDos);

    display_connectResultMessage.innerText = "loading evening to-do list...";
    await requestNameCharacteristic.writeValueWithResponse(encoder.encode("numEveningToDos\0"));
    await portalSideRequestCharacteristic.writeValue(trueBuffer);
    result = await dataCharacteristic.readValue();
    config_numEveningToDos = decoder.decode(result);
    console.log("Num Evening To-Dos: " + config_numEveningToDos);

    await requestNameCharacteristic.writeValueWithResponse(encoder.encode("eveningToDo\0"));
    for(var i = 0; i < config_numEveningToDos; i++) {
        const uint8 = new Uint8Array(1);
        uint8[0] = i;
        await indexCharacteristic.writeValue(uint8);
        await portalSideRequestCharacteristic.writeValue(trueBuffer);
        result = await dataCharacteristic.readValue();
        config_eveningToDos.push(decoder.decode(result));
    }
    console.log("Evening To-Dos: " + config_eveningToDos);

    updateSiteFromValues();
    displaySelectedFeatures();
    goMain();
}

function updateSiteFromValues() {
    document.querySelector("input[name='theme'][value='" + config_theme + "'").checked = true;
    display_firstName.innerText = config_firstName;
    display_lastName.innerText = config_lastName;

    let index = 0;
    for(const c of config_features) {
        switch(index++) {
            case 0:
                todoCheckBox.checked = (c == '1');
                break;
            case 1:
                weeklyCalCheckBox.checked = (c == '1');
                break;
            case 2:
                moodCheckBox.checked = (c == '1');moodCheckBox
                break;
            case 3:
                weatherCheckBox.checked = (c == '1');
                break;
            case 4:
                gameCheckBox.checked = (c == '1');
                break;
            case 5:
                quotesCheckBox.checked = (c == '1');
                break;
            case 6:
                themeCheckBox.checked = (c == '1');
                break
            default:
                break;
            
        }
    }
}






// 1234567
// order: Todo[1], WeeklyCalendar[2], Quotes[3], Weather[4], Game[5], Mood[6], Theme[7]

function displaySelectedFeatures() {
    todoPage.style.display = todoCheckBox.checked ? "block" : "none";
    eventsPage.style.display = weeklyCalCheckBox.checked ? "block" : "none";
    themePage.style.display = themeCheckBox.checked ? "block" : "none";
    weatherPage.style.display = weatherCheckBox.checked ? "block" : "none";
    ticTacToePage.style.display = gameCheckBox.checked ? "block" : "none";
    moodTrackerPage.style.display = moodCheckBox.checked ? "block" : "none";
    dailyQuotePage.style.display = quotesCheckBox.checked ? "block" : "none";
}

function updateFeatures() {
    displaySelectedFeatures();

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

async function updateTheme() {
    console.log("Updating theme...");
    var checkedBox = document.querySelector('input[name="theme"]:checked');
    if(checkedBox == null) {
        console.log("Update failed! No theme selected.");
        return;
    }
    var selectedTheme = checkedBox.value + '\0';
    console.log("New theme value: " + selectedTheme);
    console.log("Sending update...");
    await dataCharacteristic.writeValueWithResponse(encoder.encode(selectedTheme));
    await requestNameCharacteristic.writeValueWithResponse(encoder.encode("theme\0"));
    await portalHasUpdateCharacteristic.writeValue(trueBuffer);
    console.log("Theme updated successfully!");
}

function themeSelected() {
var selectedTheme = document.querySelector('input[name="theme"]:checked');

if (selectedTheme) {
    var themeSelectedValue = selectedTheme.value;
    console.log(themeSelectedValue);
}
}
function showDateBoxTodo(){
var selectedDateOrDaily = document.querySelector('input[name="date"]:checked');
var valueDateorDaily = selectedDateOrDaily.value;

if (valueDateorDaily == 1) {
    todoDateTitle.style.display = "inline";
    todoDate.style.display = "inline";

}else{
    todoDateTitle.style.display = "none";
    todoDate.style.display = "none";
}

}
function getTask() {
if(todoDate.style.display == "inline"){
    console.log(todoDate.value);
}
else{
    console.log("daily");
}
console.log(todoName.value);
}
function showDateBoxEvent() {
    var selectedDateOrDaily = document.querySelector('input[name="date"]:checked');
    var valueDateorDaily = selectedDateOrDaily.value;

    if (valueDateorDaily == 1) {
        eventDateTitle.style.display = "inline";
        eventDate.style.display = "inline";
    } else {
        eventDateTitle.style.display = "none";
        eventDate.style.display = "none";
    }
}
function getEvent() {
    if (eventDate.style.display == "inline") {
        console.log(eventDate.value);
    } else {
        console.log("daily");
    }
    console.log(eventName.value);
}

function getEventDateNameTime(){
    var eForm = document.getElementById("Event-Info-Form");
    var eName = eventName.value;
    var eventDateTime = document.getElementById("eventDateTime").value;
    var eventDate = eventDateTime.split("T")[0];
    var eventTime = eventDateTime.split("T")[1];

    console.log(eName);
    console.log(eventDate);
    console.log(eventTime);
    eForm.reset();
    return false;
}


//These are functions to "change pages"
function goLanding() {
    mainLogo.style.display = "inline";
    needsBluetoothScreen.style.display = "block";
    featuresPage.style.display = "none";
    boardDisplayPage.style.display = "none";
    profilePage.style.display = "none";
}

function goMain() {
    mainLogo.style.display = "inline";
    needsBluetoothScreen.style.display = "none";
    featuresPage.style.display = "flex";
    boardDisplayPage.style.display = "none";
    profilePage.style.display = "none";
}

function goBoardDisplay() {
    mainLogo.style.display = "none";
    needsBluetoothScreen.style.display = "none";
    navbar.style.display = "block";
    dotContainer.style.display = "none";
    featuresPage.style.display = "none";
    boardDisplayPage.style.display = "block";
    profilePage.style.display = "none";
}

function goProfile() {
    mainLogo.style.display = "none";
    needsBluetoothScreen.style.display = "none";
    navbar.style.display = "block";
    dotContainer.style.display = "none";
    featuresPage.style.display = "none";
    boardDisplayPage.style.display = "none";
    profilePage.style.display = "block";

}

function getZip(){
    var zipCode = document.getElementById("zip").value;

    alert(zipCode);
}
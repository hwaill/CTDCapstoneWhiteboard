
var navbar = document.getElementById("navbar");
var mainLogo = document.getElementById("mainLogo");
var needsBluetoothScreen = document.getElementById("needsBluetoothConnection");
var connectResultMessaage = document.getElementById("connectionResultMessage");

var dotContainer = document.getElementById("dot-container");

var featuresPage = document.getElementById("featuresSection");
var boardDisplayPage = document.getElementById("boardDisplayPage");
var profilePage = document.getElementById("profilePage");

var todoPage = document.getElementById("todoPage");
var eventsPage = document.getElementById("eventsPage");
var themePage = document.getElementById("themePage");

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

var todoDateTitle = document.getElementById("todoDate-label");
var todoDate = document.getElementById("todoDate");
var todoName = document.getElementById("todoName");

var eventDateTitle = document.getElementById("eventDate-label");
var eventDate = document.getElementById("eventDate");
var eventName = document.getElementById("eventName");

todoDateTitle.style.display = "none";
todoDate.style.display = "none";

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

goMain();

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
    connectResultMessaage.innerText = "connecting...";
    const server = await device.gatt.connect();
    const service = await server.getPrimaryService(serviceUUID);

    console.log("test");
    dataCharacteristic = await service.getCharacteristic(dataCharacteristic1UUID);
    requestNameCharacteristic = await service.getCharacteristic(requestNameUUID);
    indexCharacteristic = await service.getCharacteristic(indexCharacteristicUUID);
    portalSideRequestCharacteristic = await service.getCharacteristic(portalSideRequestUUID);
    portalHasUpdateCharacteristic = await service.getCharacteristic(portalHasUpdateUUID);
    successResponseCharacteristic = await service.getCharacteristic(successResponseUUID);
    
    connectResultMessaage.innerText = "loading first name...";
    await requestNameCharacteristic.writeValueWithResponse(encoder.encode("firstName\0"));
    await portalSideRequestCharacteristic.writeValue(trueBuffer);
    result = await dataCharacteristic.readValue();
    config_firstName = decoder.decode(result);
    console.log("Last name: " + config_firstName);

    connectResultMessaage.innerText = "loading last name...";
    await requestNameCharacteristic.writeValueWithResponse(encoder.encode("lastName\0"));
    await portalSideRequestCharacteristic.writeValue(trueBuffer);
    result = await dataCharacteristic.readValue();
    config_lastName = decoder.decode(result);
    console.log("Last name: " + config_lastName);

    connectResultMessaage.innerText = "loading theme preference...";
    await requestNameCharacteristic.writeValueWithResponse(encoder.encode("theme\0"));
    await portalSideRequestCharacteristic.writeValue(trueBuffer);
    result = await dataCharacteristic.readValue();
    config_theme = decoder.decode(result);
    console.log("Theme: " + config_theme);

    connectResultMessaage.innerText = "loading enabled features...";
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

    connectResultMessaage.innerText = "loading mood tracking preferences...";
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

    connectResultMessaage.innerText = "loading wifi credentials...";
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

    connectResultMessaage.innerText = "loading time zone...";
    await requestNameCharacteristic.writeValueWithResponse(encoder.encode("timeZone\0"));
    await portalSideRequestCharacteristic.writeValue(trueBuffer);
    result = await dataCharacteristic.readValue();
    config_timeZone = decoder.decode(result);
    console.log("Time Zone: " + config_timeZone);

    connectResultMessaage.innerText = "loading zip code...";
    await requestNameCharacteristic.writeValueWithResponse(encoder.encode("zipCode\0"));
    await portalSideRequestCharacteristic.writeValue(trueBuffer);
    result = await dataCharacteristic.readValue();
    config_zipCode = decoder.decode(result);
    console.log("Zip Code: " + config_zipCode);

    connectResultMessaage.innerText = "loading morning to-do list...";
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

    connectResultMessaage.innerText = "loading daytime to-do list...";
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

    connectResultMessaage.innerText = "loading evening to-do list...";
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
    goDOT();
}

function updateSiteFromValues() {
    document.querySelector("input[name='theme'][value='" + config_theme + "'").checked = true;
}






// 1234567
// order: Todo[1], WeeklyCalendar[2], Quotes[3], Weather[4], Game[5], Mood[6], Theme[7]

function updateFeatures() {

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


//These are functions to "change pages"
function goMain() {
    mainLogo.style.display = "inline";
    needsBluetoothScreen.style.display = "block";
    navbar.style.display = "none";
    dotContainer.style.display = "none";
    featuresPage.style.display = "none";
    boardDisplayPage.style.display = "none";
    profilePage.style.display = "none";
}

function goDOT() {
    mainLogo.style.display = "inline";
    needsBluetoothScreen.style.display = "none";
    navbar.style.display = "none";
    dotContainer.style.display = "block";
    featuresPage.style.display = "none";
    boardDisplayPage.style.display = "none";
    profilePage.style.display = "none";
}

function goFeatures() {
    mainLogo.style.display = "none";
    needsBluetoothScreen.style.display = "none";
    navbar.style.display = "block";
    dotContainer.style.display = "none";
    featuresPage.style.display = "block";
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
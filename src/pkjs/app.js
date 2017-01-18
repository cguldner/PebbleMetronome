// Import the Clay package
var Clay = require('pebble-clay');
// Load our Clay configuration file
var claySettings = require('./config');
// Initialize Clay
var clay = new Clay(claySettings.clayConfig, claySettings.customClay);
/**
  * The format of the configuration page for the app, for use with Clay
  */

/**
 * Shows the vibrate length slider if the Vibrate toggle is checked
 */
function customClay(minified) {
    var clayConfig = this;

    // Show/hide the vibrate length based on the Vibrate toggle switch
    function toggleVibrateLength() {
        if (this.get())
            clayConfig.getItemByMessageKey('VibeLength').show();
        else
            clayConfig.getItemByMessageKey('VibeLength').hide();
    }
    // Show/hide the Vibrate switch based on the meter arm switch
    function toggleVibrateSwitch() {
        if (this.get())
            clayConfig.getItemByMessageKey('Vibrate').show();
        else
            clayConfig.getItemByMessageKey('Vibrate').hide();
    }

    clayConfig.on(clayConfig.EVENTS.AFTER_BUILD, function() {
        var vibrateToggle = clayConfig.getItemByMessageKey('Vibrate');
        toggleVibrateLength.call(vibrateToggle);
        vibrateToggle.on('change', toggleVibrateLength);
        
        var armToggleToggle = clayConfig.getItemByMessageKey('MeterArm');
        toggleVibrateSwitch.call(armToggleToggle);
        armToggleToggle.on('change', toggleVibrateSwitch);
    });
}

var clayConfig = [
    {
        "type": "heading",
        "defaultValue": "Metronome Configuration"
    },
    {
        "type": "text",
        "defaultValue": "You can edit the settings for the app here!"
    },
    {
        "type": "section",
        "items": [
            {
                "type": "heading",
                "defaultValue": "Colors"
            },
            {
                "type": "color",
                "messageKey": "ForegroundColor",
                "defaultValue": "000000",
                "allowGray" : true,
                "label": "Text Color"
            },
            {
                "type": "color",
                "messageKey": "BackgroundColor",
                "defaultValue": "FFFFFF",
                "allowGray" : true,
                "label": "Background Color"
            }
        ]
    },
    {
        "type": "section",
        "items": [
            {
                "type": "heading",
                "defaultValue": "More Settings"
            },
            {
                "type": "toggle",
                "messageKey": "Flashing",
                "label": "Enable flashing of colors",
                "defaultValue": false
            },
            {
                "type": "toggle",
                "messageKey": "MeterArm",
                "label": "Show the ticking arm that moves back and forth",
                "defaultValue": true
            },
            {
                "type": "toggle",
                "messageKey": "Vibrate",
                "label": "Enable vibrating",
                "defaultValue": true
            },
            {
                "type": "slider",
                "messageKey": "VibeLength",
                "defaultValue": 50,
                "label": "Vibration Length",
                "description": "In Milliseconds",
                "min": 0,
                "max": 150,
                "step": 1
            }
        ]
    },
    {
        "type": "submit",
        "defaultValue": "Save Settings"
    }
];

module.exports = {clayConfig: clayConfig, customClay: customClay};
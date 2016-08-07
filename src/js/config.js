/**
  * The format of the configuration page for the app, for use with Clay
  */
module.exports = [
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
                "label": "Enable flashing",
                "defaultValue": true
            },
            {
                "type": "slider",
                "messageKey": "VibeLength",
                "defaultValue": 50,
                "label": "Vibration Length",
                "description": "In Milliseconds",
                "min": 20,
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

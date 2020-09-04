# Slicky GUI
Cross-platform GUI for our Do Not Disturb light, <a href="https://www.lexcelon.com/slicky">Slicky</a>

## What is Slicky?
<p>Slicky is a do-not-disturb desk light designed to notify your coworkers in the office or your spouse and children in the home that you are either busy and not to be disturbed or available to talk! It aids your productivity, allowing you to work distraction-free until you are ready for interruptions.
Connect Slicky to a standard USB port on your laptop or desktop to turn it on and update your color status. We provide a computer application for Windows, MacOS, and Linux to change your status/color on our website.</p>
<p>Choose from pre-set statuses/colors, including Do Not Disturb (red), Proceed with Caution (yellow), and Available (green) or create your own! Slicky comes with a magnet on its underside so you can mount it easily to your cubicle or wherever it can get visibility!</p>
<p>Slicky can also be integrated with Slack using the GUI (computer application). Once connected, you can automatically set your Slack status when you change your Slicky status/color.</p>

## Building
You don't need to do much after cloning the repo to run the GUI. Using QtCreator, update the `client_id` and `client_secret` variables with the real values found on api.slack.com in *SlackDialog.h*. Make sure to also update the encryption seed numbers in *slackdialog.cpp* and *mainwindow.cpp*.


## Licensing
This project is provided open source via the GNU GPL 3.0 license. You can find more info on the license within the repo folder. This project was made using the Qt Open Source version. Everything is available as-is.
# Electrical-SR20

*Welcome to e-tron. We think you'll like it here.*


## Branch Structure & Pull Procedure

##### User Branches
All changes and additions to the electrical repository __must originate in user
branches.__ A new user branch should be created for each independent change. For example,
if I wanted to update the PDM code, and the ATCC code, I would make two
_separate user branches_ for each change (unless the change was
the same for both modules, ex. a DBC structure update). The structure for a new
user branch is: `user/*your_username*/YYYY-MM-DD/short_description`. So, if I wanted to make
all of the driver display LED's green and white for a test, my user branch would look something like this:
`user/TheYonkk/2019-10-09/DD_green&white_LED`. It would be good practice to include the module abbreviation
in the beginning of your description, however not all changes are to modules, and therefore it is not required.

##### Development Branches
Development (dev) branches are to be made at regular intervals to store code changes that have been more-or-less
finalized within a user branch, but have not been verified (more on verified changes in the *master section*). Let's
say that  I like my green and white LED code from above, and I'm ready to test it on car . From there, I'll create a
pull request into the latest dev branch. This means that my code has been written, I know that it compiles fine, I've
done all of the tests that I can, and that it should be ready to be put on car. The format for a dev branch is:
`dev/YYYY-MM-DD`. Note that there is no qualitative statement in the branch name, this is because dev branches will
likely have more than one change in development at a time. In my example, I'd create a request to pull my user branch,
`user/TheYonkk/2019-10-09/DD_green&white_LED`, into the latest dev branch, `dev/2019-10-05`.

##### Master Branch
The master branch should only be used for verified code that is reliable and can be uploaded to the car at any given
moment without a doubt of proper functionality. Let's say we're at FSAE Michigan and tight for time before the next
event. We made a change to the code in order to test something and now the module that the code was for is
malfunctioning. We need to know exactly where the reliable code is and how to access it quickly, which is what the
master branch is for. The branch itself is easy to find, as it's name is `master`.

A code becomes verified once it has been tested in an operational environment and approved by either Dave, Nick, or
both. This could be as simple as making sure that the ATCC can send CAN messages, or, as complex as verifying that the
PDM has acceptable fan and water pump control at low battery voltages and high engine temperatures.

## File Structure
All new files within a branch should follow the pre-existing file structure. Furthermore, each module, project, or
script should have a `README.md` file for important information regarding the associated contents.

## Code Style Guide
The most common style guide for c++ code is the
[Google Style Guide](https://google.github.io/styleguide/cppguide.html). We will not be using the Google Style Guide
word-for-word, however, you should make your best attempt to follow it. CSE 232 uses the Google Style Guide, so if you
haven't taken it yet, this is a great opportunity to get ahead of the pack. One deviation from the guide is the line
length limit. Limits are important, but 80 is just too short. We will use a line length limit of 120, and anything
after that length should wrap around to the next line (In Atom, you can change the preferred length in: File >
Settings > Editor > Preferred line length). Uniformity to code is the one of the keys to understandability.
If you look back at older code from the team, you'll probably struggle to understand it. This was because there was no
style guide. The style guide is important. Always follow the style guide.

Here are some things that you should change in Atom to be compliant with the style guide:
* Preferred line length: 120 (as mentioned above)
* Tab size: 2 (preferences > settings > editor > tab length)

## Software Flow

##### Arduino IDE
Our code is compiled in the [Arduino IDE](https://www.arduino.cc/en/Main/Software). If you're on Windows, look for the
non-windows-app version.

Because we are not savages, and we use Atom as our preferred text editor, we need to tell Arduino that the code is being updated elsewhere. That way, we don't have to close and reopen a program file every time we want to flash some new updates to a microcontroller. To do this, navigate to your settings page in Arduino, and check the box `Use external editor`.

##### Teensyduino
The [Teensyduino](https://www.pjrc.com/teensy/teensyduino.html) Arduino add-on adds the Teensy microcontroller boards
to Arduino, as well as a program the enables communication and programming via a USB bootloader.

##### Atom
[Atom](https://atom.io/) is a popular text editor that you will probably spend most of your time in. It's 100%
customizable, so you can change themes, plugins, and configurations to suit your needs exactly. Dave uses the
[Monokai](https://atom.io/themes/monokai) theme because he likes fruity colors, but you can use whatever you want. A
few packages that we highly recommend:
* [Linter](https://atom.io/packages/linter)
* [Minimap](https://atom.io/packages/minimap)
* [File Icons](https://atom.io/packages/file-icons)
* [autosave](https://atom.io/packages/autosave)

# Github Game-Off 2021

## **Build Instructions**

```bash
# Open a terminal in the desired directory
# clone the repository while init submodules
git clone --recurse-submodules https://github.com/LawG4/GameOff2021.git
```

Now go ahead and download one of the dependencies we can't include in this repo
irrKlang, it is the sound engine. Found at the following URL : https://www.ambiera.com/irrklang/downloads.html
Download the 64 bit version and extract it into GameOff2021/External/irrKlang

```bash
# Move back into the terminal you had open
# Move into the repo
cd GameOff2021

# Run the build commands
mkdir build
cd build
cmake ..
cmake --build .
```


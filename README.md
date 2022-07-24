
## Dependencies

Before compile the application, make sure you have GIT and CMAKE on you machine

## Deployment

To deploy this project run these commands.

### Windows MingW
```bash
  mkdir build
  cd build
  cmake -G "MinGW Makefiles" ..
  mingw32-make.exe
```
### Linux GCC
```bash
  mkdir build
  cd build
  cmake ..
  make
```
## Usage/Examples
You can find the executable in `build/install/bin`
Just run the program.
```javascript
./ReactiveCurl
```


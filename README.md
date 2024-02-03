# AstragaramLibs

### Build this application using conan 2

#### First install conan in your linux development machine
> - sudo apt-get update
> - sudo apt install python3-pip
> - sudo pip install conan==2.0.17

#### Create default conan profile in your development machine
> - conan profile detect --name default

#### Now use following steps to compile code
> - conan install . --output-folder build
> - conan build . --output-folder build


[![Contributors][contributors-shield]][contributors-url]
[![Pull Requests][pull-requests-shield]][pull-requests-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stargazers-shield]][stargazers-url]
[![Issues][issues-shield]][issues-url]
[![Watchers][watchers-shield]][watchers-url]
[![LinkedIn][linkedin-shield]][linkedin-url]

[contributors-shield]: https://img.shields.io/github/contributors/TechPrastu/AstragaramLibs.svg?style=for-the-badge
[contributors-url]: https://github.com/TechPrastu/AstragaramLibs/graphs/contributors
[pull-requests-shield]: https://img.shields.io/github/issues-pr/TechPrastu/AstragaramLibs.svg?style=for-the-badge
[pull-requests-url]: https://github.com/TechPrastu/AstragaramLibs/pulls
[forks-shield]: https://img.shields.io/github/forks/TechPrastu/AstragaramLibs.svg?style=for-the-badge
[forks-url]: https://github.com/TechPrastu/AstragaramLibs/network/members
[stargazers-shield]: https://img.shields.io/github/stars/TechPrastu/AstragaramLibs.svg?style=for-the-badge
[stargazers-url]: https://github.com/TechPrastu/AstragaramLibs/stargazers
[issues-shield]: https://img.shields.io/github/issues/TechPrastu/AstragaramLibs.svg?style=for-the-badge
[issues-url]: https://github.com/TechPrastu/AstragaramLibs/issues
[watchers-shield]: https://img.shields.io/github/watchers/TechPrastu/AstragaramLibs.svg?style=for-the-badge
[watchers-url]: https://github.com/TechPrastu/AstragaramLibs/watchers
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://www.linkedin.com/in/jigneshkumar-vadaviya/


# AstragaramLibs

### Build this application using conan 2

#### First install conan in your linux development machine
> - sudo apt-get update
> - sudo apt install python3-pip
> - sudo pip install conan==2.11.0

#### Create default conan profile in your development machine
> - conan profile detect --name default

#### Following steps to compile code
> - conan install . -u
> - conan build . 
> - conan export-pkg . 

#### Following step to Create Conan Package
> - conan create . --build=missing -u

pipeline {

    agent any

    environment {
        NAME = 'Trittium'
        BASE_NAME = 'trittium'
        ZIP_NAME = 'Trittium'
    }

    stages {

        stage("depends") {

            steps {
                echo 'building depends ...'
                sh '''#!/bin/bash
                    cd depends
                    make -j $(nproc) HOST=x86_64-pc-linux-gnu
                    make -j $(nproc) HOST=x86_64-w64-mingw32
                    make -j $(nproc) HOST=aarch64-linux-gnu
                    rm -rf SDKs
                    mkdir SDKs
                    cd SDKs
                    wget -nc https://github.com/phracker/MacOSX-SDKs/releases/download/10.15/MacOSX10.11.sdk.tar.xz
                    tar -xf MacOSX10.11.sdk.tar.xz
                    rm MacOSX10.11.sdk.tar.xz
                    cd ..
                    make -j $(nproc) HOST=x86_64-apple-darwin14
                '''
            }
        }

        stage("build_linux") {

            steps {
                echo 'building linux ...'
                sh '''#!/bin/bash
                    make clean
                    ./autogen.sh
                    ./configure --enable-glibc-back-compat --prefix=$(pwd)/depends/x86_64-pc-linux-gnu LDFLAGS="-static-libstdc++" --enable-cxx --enable-static --disable-shared --disable-debug --disable-tests --disable-bench --with-pic CPPFLAGS="-fPIC -O3" CXXFLAGS="-fPIC -O3"
	                make -j $(nproc) HOST=x86_64-pc-linux-gnu
                '''
            }
        }

        stage("deploy_linux") {

            steps {
                echo 'deploy linux ...'

                sh """#!/bin/bash
                    mkdir -p deploy/linux
                    cp src/${BASE_NAME}d src/${BASE_NAME}-cli src/${BASE_NAME}-tx src/qt/${BASE_NAME}-qt deploy/linux/
                    cd deploy/linux
                    zip ${ZIP_NAME}-\$(git describe --abbrev=0 --tags | sed s/v//)-Linux.zip ${BASE_NAME}d ${BASE_NAME}-cli ${BASE_NAME}-tx ${BASE_NAME}-qt
                    rm -f ${BASE_NAME}d ${BASE_NAME}-cli ${BASE_NAME}-tx ${BASE_NAME}-qt
                """
            }
        }

        stage("build_windows") {

            steps {
                echo 'building windows ...'
                sh '''#!/bin/bash
                    make clean
                    ./autogen.sh
                    ./configure --prefix=$(pwd)/depends/x86_64-w64-mingw32 --disable-debug --disable-tests --disable-bench --disable-online-rust CFLAGS="-O3" CXXFLAGS="-O3"
	                make -j $(nproc) HOST=x86_64-w64-mingw32
                '''
            }
        }

        stage("deploy_windows") {

            steps {
                echo 'deploy windows ...'
                sh """#!/bin/bash
                    mkdir -p deploy/windows
                    cp src/${BASE_NAME}d.exe src/${BASE_NAME}-cli.exe src/${BASE_NAME}-tx.exe src/qt/${BASE_NAME}-qt.exe deploy/windows/
                    cd deploy/windows
                    zip ${ZIP_NAME}-\$(git describe --abbrev=0 --tags | sed s/v//)-Windows.zip ${BASE_NAME}d.exe ${BASE_NAME}-cli.exe ${BASE_NAME}-tx.exe ${BASE_NAME}-qt.exe
                    rm -f ${BASE_NAME}d.exe ${BASE_NAME}-cli.exe ${BASE_NAME}-tx.exe ${BASE_NAME}-qt.exe
                """
            }
        }

        stage("build_macos") {

            steps {
                echo 'building macos ...'
                sh '''#!/bin/bash
                    make clean
                    ./autogen.sh
                    ./configure --prefix=$(pwd)/depends/x86_64-apple-darwin14 --enable-cxx --enable-static --disable-shared --disable-debug --disable-tests --disable-bench --disable-online-rust
                    make -j $(nproc) HOST=x86_64-apple-darwin14
                '''
            }
        }

        stage("deploy_macos") {

            steps {
                echo 'deploy macos ...'
                sh """#!/bin/bash
                    make deploy HOST=x86_64-apple-darwin14
                    mkdir -p deploy/macos
                    cp src/${BASE_NAME}d src/${BASE_NAME}-cli src/${BASE_NAME}-tx src/qt/${BASE_NAME}-qt ${NAME}-Core.dmg deploy/macos/
                    cd deploy/macos
                    zip ${ZIP_NAME}-\$(git describe --abbrev=0 --tags | sed s/v//)-MacOS.zip ${BASE_NAME}d ${BASE_NAME}-cli ${BASE_NAME}-tx ${BASE_NAME}-qt ${NAME}-Core.dmg
                    rm -f ${BASE_NAME}d ${BASE_NAME}-cli ${BASE_NAME}-tx ${BASE_NAME}-qt ${NAME}-Core.dmg
                """
            }
        }

        stage("build_aarch64") {

            steps {
                echo 'building aarch64 ...'
                sh '''#!/bin/bash
                    make clean
                    ./autogen.sh
                    ./configure --prefix=$(pwd)/depends/aarch64-linux-gnu --disable-debug --disable-tests --disable-bench --disable-online-rust CFLAGS="-O3" CXXFLAGS="-O3"
	                make -j $(nproc) HOST=aarch64-linux-gnu
                '''
            }
        }

        stage("deploy_aarch64") {

            steps {
                echo 'deploy aarch64 ...'
                sh """#!/bin/bash
                    mkdir -p deploy/aarch64
                    cp src/${BASE_NAME}d src/${BASE_NAME}-cli src/${BASE_NAME}-tx src/qt/${BASE_NAME}-qt deploy/aarch64/
                    cd deploy/aarch64
                    zip ${ZIP_NAME}-\$(git describe --abbrev=0 --tags | sed s/v//)-aarch64.zip ${BASE_NAME}d ${BASE_NAME}-cli ${BASE_NAME}-tx ${BASE_NAME}-qt
                    rm -f ${BASE_NAME}d ${BASE_NAME}-cli ${BASE_NAME}-tx ${BASE_NAME}-qt
                """
            }
        }

        stage("build_aarch32") {

            steps {
                echo 'building aarch32 ...'
                sh '''#!/bin/bash
                    make clean
                    ./autogen.sh
                    ./configure --prefix=$(pwd)/depends/arm-linux-gnueabihf --disable-debug --disable-tests --disable-bench --disable-online-rust CFLAGS="-O3" CXXFLAGS="-O3"
	                make -j $(nproc) HOST=arm-linux-gnueabihf
                '''
            }
        }

        stage("deploy_aarch32") {

            steps {
                echo 'deploy aarch32 ...'
                sh """#!/bin/bash
                    mkdir -p deploy/aarch32
                    cp src/${BASE_NAME}d src/${BASE_NAME}-cli src/${BASE_NAME}-tx src/qt/${BASE_NAME}-qt deploy/aarch32/
                    cd deploy/aarch32
                    zip ${ZIP_NAME}-\$(git describe --abbrev=0 --tags | sed s/v//)-aarch32.zip ${BASE_NAME}d ${BASE_NAME}-cli ${BASE_NAME}-tx ${BASE_NAME}-qt
                    rm -f ${BASE_NAME}d ${BASE_NAME}-cli ${BASE_NAME}-tx ${BASE_NAME}-qt
                """
            }
        }
    }
}
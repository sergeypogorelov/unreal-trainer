# About

This application is an Unreal Engine client for training AI models. It consists of a simple game containing one simple gameplay mechanic to collect rewards for specific period of time and tools to train a bot playing the game. The app relies on a server which will be published later.

## Usage

You can configure gameplay and training via data assets stored in `/Content/Common` folder. There should be two files such as `DA_GamePlaySettings` and `DA_TrainingSettings`.

Asset `DA_GamePlaySettings` is responsible for gameplay settings. For example, you can change duration of one round. In order to do that, change `Duration of one step` (seconds) and `Count of steps`.

Asset `DA_TrainingSettings` is responsible for training settings. There you can change count of training episodes, evaluation episodes, count of agents and many other options.

Training areas, bots and rewards are generated procedurally depending on the settings above.

## Prerequisite

Before moving forward, make sure you have installed the following applications:

 - Server prerequisite (will be published later);
 - Unreal Engine (version 5.1);
 - Visual Studio 2019 or Visual Studio 2022;

 For more details on how to set up Visual Studio for Unreal Engine please visit corresponding [docs](https://docs.unrealengine.com/5.1/en-US/setting-up-visual-studio-development-environment-for-cplusplus-projects-in-unreal-engine/).

## Cloning

This repository uses git modules so it's recommended to use "--recurse-submodules" parameter while cloning.

    git clone --recurse-submodules https://github.com/sergeypogorelov/unreal-trainer.git

## Running

Once you have cloned the repository, open file `UnrealTrainer.uproject` in the root folder via Unreal Engine. At first start, you will have to confirm building modules.

## Development

Once you have cloned the repository, open the root folder and generate Visual Studio files. It can be done by openning context menu on file `UnrealTrainer.uproject` and then clicking `Generate Visual Studio project files`. Once it's completed, open file `UnrealTrainer.sln` via Visual Studio and build the source. Now you are ready to debug.

# Markdown Asset

> * Tested on UE 5.3
> * Note this also requires the Web Browser plugin to be enabled.

This adds markdown as an asset to Unreal.

Why would anyone want to do this you ask? That is a good question!

Well, when working on side projects I like to put documentation directly in the project. Now I can, straight in the editor. Is this a good thing? Who knows. Mainly I just wanted to write my own asset type and this seemed like a good idea at the time. Regrets? I have a few. But now it is done and other people can use it if they want, and I won't feel it was a complete waste of time :)

## How to install

1. Clone this repo into your project's `Plugins` folder.
2. Build your project
3. Enable the Web Browser plugin (`Edit -> Plugins` from the menu)
4. Enable the Markdown Asset plugin (`Edit -> Plugins` from the menu)

Hopefully that should work.

## How to use

### Create an asset

* In the content browser
* Right click -> Miscellaneous -> Markdown Asset

![X](./Docs/NewAsset.png)

### Edit an asset

* Double click the asset to edit
* This will open the asset in the editor
* You can switch between the editor and preview mode using the button in the top right

![X](./Docs/Editing.png)

### Settings

* You can swap between a light and dark skin in the editor preferences
* Edit -> Editor Preferences -> Plugins -> Markdown Asset

## Markdown

The editor runs inside the unreal web browser client. Unfortunately, this seems to be restricted and certain features are not present.

However, under the hood it is using [markdown-it](https://github.com/markdown-it/markdown-it) and I've enabled a bunch of plugins that extend the markdown language with some extra useful features, listed below:

### Syntax Highlighting

Should be automatic for code blocks. You can specify the language like this:

    ```c++
    #include "TestGamePlayerController.h"
    #include "EnhancedInputSubsystems.h"

    void ATestGamePlayerController::BeginPlay()
    {
        Super::BeginPlay();
    }
    ```

### Tasks Lists

Are added like this:

```markdown
- [x] fix closing browser warning
- [x] add custom icon
- [ ] better editing experience
- [ ] add dark and light themes
```

Which gives this:

![X](./Docs/TaskList.png)



### Anchors

Headings are automatically given anchors, so you can link to them like this:

```markdown
[Link](#ignore-extra-cells)

will jump to ...

## Ignore extra cells
```

### Table of Contents

You can automatically add a table of contents to your document like this:

```markdown
[[toc]]
```

### Diagrams

PlantUML and Dot diagrams seem to work. Unfortunately Mermaid does not appear to.

You add them like this:


    ```plantuml
    Bob -> Alice : hello
    ```

    ```dot
    digraph example1 {
        1 -> 2 -> { 4, 5 };
        1 -> 3 -> { 6, 7 };
    }
    ```

Which gives this:

![X](./Docs/Diagrams.png)

See [markdown-it-textual-uml](https://github.com/manastalukdar/markdown-it-textual-uml) for more details.

## Notes

**NB:** You probably do not want to ship with your documentation. Make sure the markdown assets are not included in your build!

Unfortunately, support for MathJax and playing videos does not work with limitations of the Unreal web browser plugin.

GL & HF


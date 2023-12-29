export default `

# ToC

[[toc]]

[Link](#ignore-extra-cells)

# Tasks

- [x] Marmaid / Flowchart diagrams
- [x] Math
- [x] Test tables
- [x] Code highlighting
- [x] Anchors / table of contents

# Code

## JavaScript

\`var x=4\`{.js}

\`\`\`js
ReactDOM.createRoot(document.getElementById('root')).render(
    <ThemeProvider theme={theme}>
      <CssBaseline />
      <App />
    </ThemeProvider>
  )
\`\`\`

## C++

\`\`\`c++
#include "TestGamePlayerController.h"
#include "EnhancedInputSubsystems.h"

void ATestGamePlayerController::BeginPlay()
{
	Super::BeginPlay();

	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
}
\`\`\`

## Python
\`\`\`python
x = 5

if x > 9:
    print("Hello, World!")
\`\`\`



# Video

@[youtube](dQw4w9WgXcQ)


# Math

Pythagoran theorem is $$a^2 + b^2 = c^2$$.
 
Bayes theorem:
 
$$$
P(A | B) = (P(B | A)P(A)) / P(B)
$$$



# Diagrams

## PlantUML

\`\`\`plantuml
Bob -> Alice : hello
\`\`\`

## Dot

\`\`\`dot
digraph example1 {
    1 -> 2 -> { 4, 5 };
    1 -> 3 -> { 6, 7 };
}
\`\`\`


## Mermaid

\`\`\`mermaid
graph TD;
    A-->B;
    A-->C;
    B-->D;
    C-->D;
\`\`\`


# Tables

### 2x2

A1 | A2
-- | --
B1 | B2


### 3x3

A1|A2|A3
-|-|-
B1|B2|B3
C1|C2|C3


### Cell Alignment

Column | Column | Column
:----- | :----: | -----:
Left   | Center | Right
l  | c  | r

### Empty Cells

| | Digit | Character
| ------ | ------|----
| a      | 4     | $
| | 365   | (
| b      |       | ^  

### Escape Character

|  |
|-|
| This is a single cell containing a \\| character |


<!-- not a standard markdown feature ...

### Merged Cells

|             |          Grouping           ||
First Header  | Second Header | Third Header |
 ------------ | :-----------: | -----------: |
Content       |          *Long Cell*        ||
Content       |   **Cell**    |         Cell |

New section   |     More      |         Data |
And more      | With an escaped '\\|'         || 
-->

### Ignore Extra Cells

| Header 1  | Header 2            | Header 3  |
| --------- | ------------------- | --------- |
| Some data | Cell 2              | Cell 3    | Ignored | Ignored |
| data      | more data | 

### With Some Content

A1|![](https://via.placeholder.com/50x50)|A3
-|-|-
default|\`code\`|**bold** and *italic*
| | image ![](https://via.placeholder.com/50x50)|C3

`;

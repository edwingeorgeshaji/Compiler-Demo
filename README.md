# Mathematical Formula Compiler

## 1. Project Overview

The Mathematical Formula Compiler is a web-based application that checks the validity of mathematical expressions and equations. It uses compiler design concepts such as lexical analysis, syntax analysis, Abstract Syntax Tree (AST) generation, and semantic checking.

The application provides a frontend where users can enter mathematical formulas and view the output returned by the compiler.

## 2. Objectives

- To develop a basic mathematical formula compiler.
- To identify tokens in mathematical expressions using lexical analysis.
- To validate expressions using grammar rules and syntax analysis.
- To construct an Abstract Syntax Tree (AST) for valid formulas.
- To perform semantic checking.
- To connect a C-based compiler with a web frontend using Flask.

## 3. Technologies Used

- **C:** Implements the compiler logic and AST operations.
- **Flex:** Performs lexical analysis.
- **Bison:** Performs syntax analysis using grammar rules.
- **Python:** Connects the compiler to the web application.
- **Flask:** Provides the backend web server.
- **HTML and CSS:** Provide the frontend interface.
- **MSYS2 UCRT64:** Development environment for compiling and running the project.

## 4. Features

- Accepts mathematical expressions through a web interface.
- Recognizes numbers, identifiers, arithmetic operators, and parentheses.
- Supports arithmetic operations such as addition, subtraction, multiplication, division, and exponentiation.
- Supports mathematical functions such as `sqrt()` and `pow()`, if enabled in the compiler grammar.
- Supports mathematical equations using the equality operator.
- Checks the syntactic structure of formulas.
- Generates an Abstract Syntax Tree (AST).
- Displays compiler output through the frontend.

## 5. Project Structure

The project folder contains the following files:

```text
formula-compiler/
├── app.py
├── lexer.l
├── parser.y
├── ast.h
├── ast.c
├── semantic.h
├── semantic.c
├── parser.tab.c
├── parser.tab.h
├── lex.yy.c
├── formula.exe
├── .venv/
└── README.md
```

**Note:** `parser.tab.c`, `parser.tab.h`, `lex.yy.c`, and `formula.exe` are generated during the build process. The `.venv` folder contains the Python virtual environment and should not normally be uploaded to GitHub.

## 6. Installation and Setup

### Prerequisites

Install or configure the following:

- MSYS2 with the UCRT64 environment
- Python
- GCC
- Flex
- Bison

The project uses a Python virtual environment to keep Flask and other Python dependencies separate from the system Python installation.

### Step 1: Open the project directory

Open the MSYS2 UCRT64 terminal and navigate to the project directory:

```bash
cd /c/msys64/home/itsab/formula-compiler
```

### Step 2: Activate the virtual environment

```bash
source .venv/bin/activate
```

The terminal should display `(.venv)` when the environment is active.

Create the environment only if it does not already exist:

```bash
python -m venv .venv
```

### Step 3: Install Flask

If Flask has not already been installed in the virtual environment, run:

```bash
python -m pip install flask
```

This installation is normally required only once for the existing virtual environment.

## 7. Build the Compiler

From the project directory, run the following commands in order:

### Generate the parser

```bash
bison -d parser.y
```

### Generate the lexer

```bash
flex lexer.l
```

### Compile the program

```bash
gcc parser.tab.c lex.yy.c ast.c semantic.c -o formula.exe -lm
```

These commands generate the parser and lexer source files and compile them into `formula.exe`.

If you modify the lexer, parser, AST, or semantic-analysis source files, rebuild the compiler before testing the changes.

## 8. Run the Web Application

Ensure that the virtual environment is active and that the compiler has been built successfully.

Start the Flask application:

```bash
python app.py
```

When the server starts, open the following address in your web browser:

```text
http://127.0.0.1:5000
```

Enter a mathematical formula in the webpage and submit it. The Flask backend passes the formula to the compiler and displays the returned output.

Keep the terminal running while using the application. Press `Ctrl+C` in the terminal to stop the Flask server.

## 9. Example Inputs

The following are examples of formulas to test, provided they are supported by the implemented grammar:

| Input | Purpose |
|---|---|
| `10 + 5` | Addition |
| `(10 + 5) * 2` | Parentheses and multiplication |
| `10 / 2` | Division |
| `2 ^ 3` | Exponentiation |
| `sqrt(25)` | Square-root function |
| `pow(2, 3)` | Power function |
| `x + 5` | Identifier and number |
| `x + y = 10` | Mathematical equation |

For invalid-input testing, try an incomplete expression such as:

```text
10 + *
```

The expected response is a syntax error or an invalid-formula message, depending on the implementation.

## 10. Compiler Workflow

1. The user enters a mathematical formula in the frontend.
2. Flask receives the submitted formula.
3. The backend passes the formula to the compiled C program.
4. Flex identifies the tokens in the input.
5. Bison checks the input against the grammar.
6. The AST module constructs and represents the formula as a tree.
7. The semantic module performs the implemented semantic checks.
8. The compiler returns its output to Flask.
9. The frontend displays the result to the user.

## 11. Troubleshooting

**Flask cannot be imported**

Activate the virtual environment and install Flask if needed:

```bash
source .venv/bin/activate
python -m pip install flask
```

**The compiler executable is missing**

Run the build commands in Section 7.

**The browser cannot connect to the application**

Make sure `python app.py` is running and open `http://127.0.0.1:5000`.

**The formula is rejected**

Check whether the formula follows the grammar supported by the compiler.

**The terminal reports a missing command**

Confirm that the MSYS2 UCRT64 environment is being used and that the required compiler tools are installed.

## 12. Future Enhancements

- Evaluate valid expressions and display numerical results.
- Improve error messages for lexical, syntax, and semantic errors.
- Add more mathematical functions and constants.
- Improve the presentation of the AST in the frontend.
- Add more test cases for valid and invalid formulas.

## 13. Conclusion

The Mathematical Formula Compiler demonstrates the basic stages of compiler construction and the integration of a C-based compiler with a Python Flask web application. It provides a practical example of lexical analysis, syntax analysis, AST generation, and semantic checking through a browser-based interface.



from pathlib import Path
import subprocess

from flask import Flask, request, render_template_string

app = Flask(__name__)

PROJECT_DIR = Path(__file__).resolve().parent
COMPILER = PROJECT_DIR / "formula.exe"

PAGE = """
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Mathematical Formula Compiler</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background: #f1f5f9;
            margin: 0;
            padding: 40px 16px;
        }
        .container {
            max-width: 650px;
            margin: auto;
            background: white;
            padding: 28px;
            border-radius: 12px;
            box-shadow: 0 4px 18px #00000012;
        }
        h1 { color: #1d3557; }
        p { line-height: 1.6; }
        input {
            box-sizing: border-box;
            width: 100%;
            padding: 12px;
            margin: 10px 0;
            font-size: 16px;
        }
        button {
            padding: 12px 20px;
            background: #2563eb;
            color: white;
            border: 0;
            border-radius: 6px;
            cursor: pointer;
        }
        pre {
            white-space: pre-wrap;
            overflow-wrap: anywhere;
            background: #f1f5f9;
            padding: 16px;
            border-radius: 6px;
        }
        .examples { color: #475569; font-size: 14px; }
    </style>
</head>
<body>
    <div class="container">
        <h1>Mathematical Formula Compiler</h1>
        <p>
            Enter a mathematical expression or equation.
            Supported operations include +, -, *, /, ^, sqrt()
            and pow().
        </p>

        <form method="post">
            <label for="formula">Formula:</label>
            <input id="formula" name="formula"
                   value="{{ formula }}"
                   placeholder="Example: x + y = 10" required>
            <button type="submit">Validate Formula</button>
        </form>

        <p class="examples">
            Examples: x + 10, x = 10, x + y = 10,
            sqrt(x), pow(x, 2) + 3 = 12
        </p>

        {% if output %}
            <h2>Compiler Output</h2>
            <pre>{{ output }}</pre>
        {% endif %}
    </div>
</body>
</html>
"""


@app.route("/", methods=["GET", "POST"])
def home():
    formula = ""
    output = ""

    if request.method == "POST":
        formula = request.form.get("formula", "").strip()

        if not formula:
            output = "Please enter a formula."
        elif len(formula) > 1000:
            output = "Formula is too long. Maximum length is 1000 characters."
        elif "\n" in formula or "\r" in formula:
            output = "Please enter one formula on a single line."
        elif not COMPILER.is_file():
            output = (
                "Compiler executable not found. "
                "Compile the Lex and Yacc files first."
            )
        else:
            try:
                result = subprocess.run(
                    [str(COMPILER)],
                    input=formula + "\n",
                    text=True,
                    capture_output=True,
                    timeout=5,
                    cwd=str(PROJECT_DIR)
                )

                output = (result.stdout + result.stderr).strip()

                if not output:
                    output = "The compiler returned no output."

            except subprocess.TimeoutExpired:
                output = "The compiler took too long to respond."
            except OSError as error:
                output = f"Could not run the compiler: {error}"

    return render_template_string(
        PAGE,
        formula=formula,
        output=output
    )


if __name__ == "__main__":
    app.run(debug=True)

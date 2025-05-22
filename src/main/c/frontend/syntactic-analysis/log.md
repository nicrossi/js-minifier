# Mini-JS Frontend – Grammar Status Report
*(Bison + Flex parser for a JavaScript-like language)*

---

## 1 . Current coverage

| Feature area | Details |
|--------------|---------|
| **Programs & blocks** | `program`, nested statement lists, `{ … }` blocks |
| **Declarations** | `let` / `const` lexical declarations (incl. simple default-initializer support) and traditional `function foo(…) { … }` declarations |
| **Expressions** | • all numeric literals, string literals<br>• identifiers & member access (`a.b`) <br>• computed access (`a[expr]`) <br>• `new` expressions with/without argument list <br>• unary operators `+ - ! ~ ++ --` (pre/post) <br>• binary arithmetic `**, *, /, %, +, -` <br>• relational `< <= > >=` <br>• (strict) equality `== != === !==` <br>• comma operator |
| **Statements** | expression, `return`, `break`, `continue`, `throw`, `if / else`, traditional `for ( ; ; )`, `while (…)`, `do … while`, `try / catch / finally` |
| **Literals / collections** | array literals `[1, 2, x]` including sparse forms (empty slots) |
| **AST support** | C structs and full destructor table for every node type; semantic actions implemented in `BisonActions.c` |
| **Memory hygiene** | All nodes have matching `release…()` routines; they’re wired into Bison `%destructor` directives |
| **Error handling** | Basic fatal error for `const` without initializer; other syntax errors rely on Bison’s default recovery |

---

## 2 . What still needs attention

| Area | Missing / incomplete |
|------|----------------------|
| **Remaining S/R conflicts** | 11 *shift/reduce* conflicts remain. They are benign in practice (unary ± inside declarations and `new` vs member/sub-script) but should be reduced for a clean build. |
| **Operator set** | Logical `&& ||`, bitwise `& | ^ << >>`, assignment family `+= -= …`, `instanceof`, `in`, `??`, ternary `? :` are not yet parsed. |
| **Object literals / JSON** | `{ a:1, ["k"]: v }` not supported. |
| **Function expressions & arrow functions** | Only *declarations* are handled. Arrow / anonymous forms are pending. |
| **Classes / modules** | `class`, `import`, `export`, `super`, `this` are absent. |
| **Destructuring & spread** | `[a, ...rest] = arr` – grammar does not yet cover patterns or spreads. |
| **Automatic semicolon insertion** | Semicolons are optional only in a few places; real ASI rules are not implemented. |
| **Unicode / numeric literal variants** | Hex, binary, octal, bigint, escapes, template literals are not tokenised. |
| **Comprehensive error recovery** | Parser aborts on first error; smarter recovery is desirable for IDE use-cases. |

A bunch of conflicts that I'll fix later...

---
# ATM Management System

A terminal-based banking simulator written in C. The program models users and
bank accounts with file-backed persistence and exposes the complete workflow
through an interactive menu.

> This is an educational Zone01 project, not banking software. Credentials are
> stored as plain text, monetary values use floating-point arithmetic, and the
> bundled users and records are fictional demo fixtures. Do not enter real
> credentials or financial information.

## Features

- Register and authenticate local users.
- Create savings, current, and fixed-term accounts.
- List accounts and calculate the interest defined for each account type.
- Update account contact details.
- Deposit and withdraw funds with balance validation.
- Remove accounts and transfer ownership to another registered user.
- Persist user and account records between runs.

## Architecture

The project separates terminal navigation, authentication, and account
operations while sharing the small record model declared in `src/header.h`.

```text
src/main.c   -> menu and application flow
src/auth.c   -> registration and authentication -> data/users.txt
src/system.c -> account operations              -> data/records.txt
```

Updates that rewrite the account collection go through `data/records.tmp`
before replacing the current records file.

## Build and run

A POSIX-like terminal and a C compiler supporting the configured warning flags
are required.

```bash
make
./atm
```

Or build and launch together:

```bash
make run
```

Remove generated objects and the executable with `make fclean`.

## Demo data

The tracked fixtures are safe examples created only to demonstrate persistence:

| Username | Password |
| --- | --- |
| `demo_alex` | `change-me-alex` |
| `demo_sam` | `change-me-sam` |

Running the application changes the tracked files in `data/`. Restore them
after experimenting if you want to return to the initial demo state.

## Project structure

- `src/main.c` — top-level menus and session flow.
- `src/auth.c` — credential input and user lookup.
- `src/system.c` — account CRUD, transactions, transfers, and interest rules.
- `data/` — deliberately human-readable demo persistence files.
- `Makefile` — warning-clean build and cleanup targets.

## Verification

The current quality gate compiles every source file with warnings treated as
errors:

```bash
make fclean
make
```

The original Zone01 audit is interactive and should be exercised in a real
terminal for login, account creation, transactions, updates, removal, and
ownership transfer.

## Status and limitations

The assignment workflow is implemented and builds without warnings. Persistence
uses whitespace-delimited text, so field values cannot contain spaces. The
project intentionally omits encryption, password hashing, decimal money types,
database transactions, and concurrent access; those would be mandatory in a
real financial system.

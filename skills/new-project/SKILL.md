---
name: new-project
description: Scaffolds a new Claude Code working folder (any area of life or work, not just code) from the standard template - CLAUDE.md, PROGRESS.md, TODO.md, README.md, a dated context/ folder, .gitignore and a local Git repo. Use when the user wants to start a new project or area folder.
argument-hint: "<folder name> [parent directory]"
---

# New project folder

Arguments: $ARGUMENTS — first word is the folder name, optional second is the parent directory. If no parent is given, use the "Projects root" from the global CLAUDE.md; if that is unset, ask once.

1. Refuse to overwrite: if the target folder exists and is non-empty, stop and ask.
2. Copy everything in this skill's `template/` folder (including dotfiles) into the new folder.
3. Ask the user at most three short questions, then fill the placeholders in CLAUDE.md and README.md:
   - What is this folder for, in one sentence?
   - Anything Claude should always or never do here?
   - Which tracker files fit: PROGRESS.md, TODO.md, HOMEWORK.md? Delete the ones that don't.
4. Rename `context/YYYY.MM.DD` to today's date.
5. `git init`, then commit everything as "Initial scaffold". Do not add a remote; mention that a private remote can be added later for off-site backup.
6. Report the path and tell the user to open a new session in that folder (so its CLAUDE.md loads), and to file it in a sidebar group (Active / Paused / Waiting) in the desktop app.

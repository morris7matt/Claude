---
name: wrap-up
description: End-of-session routine. Brings CLAUDE.md, PROGRESS.md, TODO.md, HOMEWORK.md and README files in the current folder up to date with everything done this session, so the next session starts from accurate context. Use when the user says "wrap up", "end of session", or asks to update context files.
---

# Wrap up this session

Make sure CLAUDE.md and every other relevant file in this folder is fully accurate and up to date based on everything we did this session.

1. List what changed this session: decisions made, files created/moved/renamed, things finished, things left open, anything learned about how a system works.
2. Update the context files that exist here (create one only if it clearly earns its place):
   - **CLAUDE.md**: stable orientation only (what this folder is, where things live, conventions). Remove anything that is no longer true. Keep it lean.
   - **PROGRESS.md**: current state (done / next / known issues) and a new dated entry at the top of the log: `### YYYY.MM.DD` with 2–5 bullets.
   - **TODO.md**: tick off finished items, add new ones with enough context to act on cold.
   - **HOMEWORK.md**: action items from meetings or recurring sessions, if this folder uses one.
   - **README.md**: if we set something up or figured out how a system works, document it so a future session can pick it up without re-deriving it.
3. If files moved or were renamed, grep for stale references to old paths and fix them.
4. Re-read each file you edited once, checking for contradictions with the others.
5. Report in a few lines: which files changed and anything still open. Do not commit unless the user says so (or uses the "ship" / "commit no push" shorthand).

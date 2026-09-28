---
name: import-claude-export
description: Migrates Claude Chat Projects (and whatever can be recovered from Cowork) out of a claude.ai data export into Claude Code folders - one folder per Project with a lean CLAUDE.md, a memory snapshot, a chat index and verbatim Markdown copies of each chat. Also searches an export for a past chat.
argument-hint: "<path to unzipped export> [destination root]"
disable-model-invocation: true
---

# Import a Claude data export

Inputs: $ARGUMENTS — the unzipped export folder (Settings > Privacy > Export data; the download link expires after 24 hours and the export may arrive as several zips — unzip them all into one dated folder, e.g. `exports/2026.09.28/`), and optionally a destination root (default: the Projects root from the global CLAUDE.md).

This skill only reads the export and writes new files. Never delete or modify the export itself.

## 1. Survey (read-only)
List the export's files, their formats, and counts: conversations, Projects, Project instructions and memory. Report the list of Projects with chat counts and last-activity dates. Ask which Projects to migrate and which (if any) to keep in Claude Chat (e.g. a "Misc. Questions" Project for quick questions).

## 2. Per Project, create `<dest>/<project-slug>/` containing
- **CLAUDE.md** — lean: the Project's custom instructions and working preferences, rewritten as standing instructions, plus the "Where things live" section from the new-project template.
- **memory-snapshot-YYYY.MM.DD.md** — the Project's memory as Markdown, headed with: "Point-in-time record from the claude.ai export of YYYY.MM.DD. Not current status."
- **chats/INDEX.md** — one line per chat: date, title, one-line gist, link to its file.
- **chats/YYYY.MM.DD-<slug>.md** — a verbatim Markdown copy of each chat (speaker labels, timestamps; attachments noted by name).
- Distilled topic notes only for folders the user says they actively work in.

Exported chats carry no Project id. Find a Project's chats by a topic sweep over content (names, distinctive terms from the Project's instructions and memory), then show the user the proposed chat-to-Project assignment before writing.

## 3. Cowork sessions
Cowork sessions are NOT in the export. They live only in the Claude desktop app's data folder (macOS: under `~/Library/Application Support/Claude/`; Windows: under `%APPDATA%\Claude\`). If the user wants them: search that folder read-only for session/transcript files, report what exists, and with approval copy readable transcripts into the matching project's `chats/` as Markdown. Do not modify anything in the app's data folder. Do this before the user stops using Cowork.

## 4. Finish
`git init` each new folder and commit. Verify counts: chats in export assigned to each Project vs. files written. Report totals and anything unassigned.

## Searching an export instead
If the user just wants to find a past chat: grep the export for distinctive strings (names, email addresses, unusual phrases) rather than common words, and offer to turn the hit into a clean Markdown file.

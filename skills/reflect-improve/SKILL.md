---
name: reflect-improve
description: Adds a suggest-only "Reflect & improve" friction log and periodic self-review step to an existing skill or scheduled task, so it proposes its own refinements without ever editing itself.
argument-hint: "<skill or scheduled-task name or path>"
disable-model-invocation: true
---

# Add a Reflect & Improve loop

Target skill: $ARGUMENTS. Locate its SKILL.md (check `.claude/skills/`, `~/.claude/skills/`, `~/.claude/scheduled-tasks/`). If it is under Git, confirm the working tree is clean or commit the current version first, so the original is one command away.

Add a final "Reflect & improve" continuous-improvement step to it, following the spec below. Understand the full intent and adapt it to THIS skill specifically.

WHY: the skill should get sharper over time by learning from how its own runs actually go (a built-in "friction log"), WITHOUT ever silently changing its own behavior. The reflection step is a closed suggestion box; the user is the human-in-the-loop who reviews and applies changes.

NON-NEGOTIABLE RULES:
1. SUGGEST ONLY (default). The skill must NEVER edit its own SKILL.md or change any of its rules, thresholds, inputs, sources, output format, or behavior on its own. It only writes suggestions to a separate log. (If the user sets autonomy to "auto-fix small mechanical stuff only," it may directly apply low-risk mechanical fixes like a dead/changed URL or a stale date, and must log everything else as a suggestion.)
2. This reflection step is the LAST step of every run, and it runs even when a run fails or stops early; those are the runs whose friction matters most.
3. Keep it lightweight: it shouldn't bloat runtime or the skill's normal output.

WHAT TO IMPLEMENT:
A. Create `improvement-log.md` in the skill's existing data/output folder (if none, pick a stable path and tell the user). Seed it with a short header: it's a suggest-only friction + improvement log, the skill writes here but never edits itself, and how to review/apply suggestions. Newest entries on top.
B. Add the log to the skill's list of known working files, so the synthesis step reads recent entries.
C. Add a "Reflect & improve (SUGGEST ONLY)" section as the skill's final step:
   - EVERY RUN: a dated 1–3 line entry at the top of the log capturing this run's friction — tools/sources that errored or returned empty, dead or stale data, wasted steps, coverage gaps, quality observations specific to this skill. "No notable friction" is a valid entry.
   - WEEKLY SYNTHESIS (Sundays by default): read the last ~7 days of notes plus any run history, find recurring patterns, add a dated "Weekly synthesis" entry with a short prioritized list of concrete refinements, each tagged `STATUS: AWAITING REVIEW`. Do NOT apply them. A skill that runs only weekly synthesizes monthly instead.
   - HEADS-UP (at most weekly): if the synthesis surfaced notable proposals, a one-line pointer through the skill's existing output channel, sent separately from any routine digest that email filters might auto-archive. No channel → skip.
D. TAILOR everything to this skill's domain: name its actual tools, sources, paths, and realistic failure modes.
E. If the skill has a separate README, mirror a short version of the section there.
F. If a default doesn't fit (not recurring, no persistent storage — e.g. a cloud routine in a fresh container — or no output channel), don't force it: propose where the log can ride along with something that already persists (saved state, the email it sends).
G. ARCHIVING rule in the log header: at the end of each review, if the log is over ~100 KB, move resolved entries and already-synthesized friction notes into `improvement-log-archive-through-YYYY-MM.md` (starting with a note that it is history only). Keep AWAITING REVIEW, DEFERRED and DECLINED items live; copy declined/open paragraphs from archived entries verbatim into a "Carried forward from archived entries" section so ideas aren't re-proposed. If the live log is git-ignored, ignore the archive too.

When done, show exactly what was added (the new log file + the section) and confirm the skill was not otherwise changed. The user reviews later by saying "review the <skill> improvement log".

<!-- Adapted from Chad Timblin's Reflect & Improve prompt, itself inspired by Tom Johnson. -->

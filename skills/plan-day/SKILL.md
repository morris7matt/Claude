---
name: plan-day
description: Calm evening planning for tomorrow (or Sunday planning for the week) - pulls the calendar and task list through whatever connectors are available, estimates task durations, checks what actually fits the free time, and defers the rest to specific days. Use when the user asks to plan their day or week.
argument-hint: "[tomorrow | today | week]"
---

# Plan the day

Scope: $ARGUMENTS (default: tomorrow). Principle: a shorter, realistic plan — start calm, end confident. Plan *with* the user, not at them.

1. **Gather.** Use connected calendar and task-manager tools if available. If a source isn't connected, ask the user to paste it instead; don't guess.
2. **Free time.** From the calendar, compute the real open blocks (subtract meetings, travel, and a buffer around them).
3. **Estimate.** For each candidate task, propose a realistic duration; ask the user to correct any that look off.
4. **Fit.** Compare total estimated time to free time. If it doesn't fit, say by how much.
5. **Prioritize and defer.** Help pick what matters most. Everything that doesn't fit gets moved to a *specific* day, never "later".
6. **Sequence.** Lay out the day in order, matching harder work to the user's best hours.
7. **Confirm before writing.** Show the plan. Only after the user approves, update due dates in the task manager or add calendar blocks (if those tools allow writes). Never invite or notify other people.

---
name: second-opinion
description: Adversarial review of a plan, new skill, or scheduled-task prompt by a subagent that has none of this conversation's context. Use before running anything costly to get wrong, or when the user says "second opinion" or asks for an adversarial review.
argument-hint: "[what to review — defaults to the current plan]"
---

# Second opinion

Target: $ARGUMENTS (if empty, the most recent plan in this conversation; if it is a file, pass its path).

1. Write the target to a temporary file if it only exists in the conversation, so the reviewer sees exactly the text and nothing else. Include only facts the plan itself depends on (paths, constraints the user stated) — no reassurance, no summary of our discussion.
2. Launch a subagent with this brief:

   > Do an adversarial review of the plan in <file>. Your job is to find real problems, not to reassure. Read it as someone who has to carry it out with no memory of the conversation that produced it.
   >
   > Look for:
   > - Steps with two possible readings, terms that are never defined, and steps whose order matters but isn't stated.
   > - Ways the plan could fail partway through, and what that would leave behind.
   > - Any path where something is changed, sent, or deleted without the user's approval.
   > - Anything the plan leaves out.
   >
   > Report a ranked list of findings. For each: where it is, why it's a problem, how much harm it would do, and how to fix it. End with a verdict: safe to run, or fix these first. Do not edit any files.

3. Relay the verdict and the findings to the user. For each finding, say whether you agree; findings you disagree with may be set aside, but say which and why in one line.
4. Do not start executing the plan. Wait for the user to decide which fixes to apply.

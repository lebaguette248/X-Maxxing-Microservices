# Usefull Commands

## 1. Pushing code to Remote / Origin / This repo

1. `git add <dateilocation>`
2. `git commit -m "<Type of Work> : <was man gemacht hat>"`
3. `git push`

---

## 2. Branch wechseln

1. `git fetch` → Holt die aktuellen Repo Infos
2. `git switch <branch Name>`

---


## 4. Aktuellen Status prüfen

1. `git status` → Zeigt Änderungen & Staging Bereich
2. `git log` → Zeigt Commit Historie
3. `git log --oneline --graph --all` → Übersichtliche History

---

## 5. Änderungen anschauen

1. `git diff` → Nicht gestagte Änderungen
2. `git diff --staged` → Gestagte Änderungen
3. `git diff --name-only`nur Dateinamen

---

## 6. Änderungen rückgängig machen

1. `git restore <datei>` → Lokale Änderungen verwerfen
2. `git restore --staged <datei>` → Aus Staging entfernen
3. `git reset --hard` → Alles auf letzten Commit zurücksetzen ⚠️

---

## 7. Branches verwalten

1. `git branch` → Alle lokalen Branches anzeigen
2. `git branch -d <branch Name>` → Branch löschen
3. `git push origin --delete <branch Name>` → Remote Branch löschen

---

## 8. Updates vom Remote holen

1. `git pull` → Fetch + Merge
2. `git pull --rebase` → Fetch + Rebase

---


🔀 Branch mergen
1️⃣ Auf den Ziel-Branch wechseln

(Der Branch, in den gemerged werden soll, dev)
1. `git pull`
2. `git switch dev`

2️⃣ Merge ausführen
1. `git merge <branch-name>`

Beispiel:
1. `git merge 1-monitoring` 

3️⃣ Falls es keine Konflikte gibt
Git erstellt automatisch einen Merge-Commit → danach:

1. `git push`

4️⃣ Falls es Konflikte gibt 🧨
Dateien mit Konflikten bearbeiten
Konflikt-Markierungen entfernen (<<<<, ====, >>>>) <br>
Dann:

1. `git add <datei>`
2. `git commit`

Danach:

1. `git push`

package com.taskmanagement.entity;

import jakarta.persistence.*;
import lombok.*;

import java.time.LocalDate;
import java.util.ArrayList;
import java.util.List;

@Entity
@Table(name = "goals", indexes = {
    @Index(name = "idx_goal_user", columnList = "user_id"),
    @Index(name = "idx_goal_status", columnList = "status")
})
@Getter
@Setter
@NoArgsConstructor
@AllArgsConstructor
@Builder
public class Goal extends BaseEntity {

    @Column(nullable = false, length = 255)
    private String title;

    @Column(columnDefinition = "TEXT")
    private String description;

    @Enumerated(EnumType.STRING)
    @Column(nullable = false)
    @Builder.Default
    private GoalStatus status = GoalStatus.NOT_STARTED;

    @Enumerated(EnumType.STRING)
    @Column(nullable = false)
    @Builder.Default
    private Priority priority = Priority.MEDIUM;

    @Column(name = "target_date")
    private LocalDate targetDate;

    @Column(name = "completion_date")
    private LocalDate completionDate;

    @Column(name = "progress_percentage", nullable = false)
    @Builder.Default
    private Integer progressPercentage = 0;

    @Column(name = "user_id", nullable = false)
    private Long userId;

    @OneToMany(mappedBy = "goal", cascade = CascadeType.ALL, orphanRemoval = true, fetch = FetchType.LAZY)
    @Builder.Default
    @OrderBy("displayOrder ASC, createdAt ASC")
    private List<Task> tasks = new ArrayList<>();

    public enum GoalStatus {
        NOT_STARTED,
        IN_PROGRESS,
        ON_HOLD,
        COMPLETED,
        CANCELLED
    }

    public enum Priority {
        LOW, MEDIUM, HIGH, URGENT
    }

    public void addTask(Task task) {
        tasks.add(task);
        task.setGoal(this);
        recalculateProgress();
    }

    public void removeTask(Task task) {
        tasks.remove(task);
        task.setGoal(null);
        recalculateProgress();
    }

    public void recalculateProgress() {
        if (tasks.isEmpty()) {
            this.progressPercentage = 0;
            return;
        }
        long completedTasks = tasks.stream()
            .filter(Task::isCompleted)
            .count();
        this.progressPercentage = (int) ((completedTasks * 100) / tasks.size());
        
        if (this.progressPercentage == 100) {
            this.status = GoalStatus.COMPLETED;
        } else if (this.progressPercentage > 0) {
            this.status = GoalStatus.IN_PROGRESS;
        }
    }
}

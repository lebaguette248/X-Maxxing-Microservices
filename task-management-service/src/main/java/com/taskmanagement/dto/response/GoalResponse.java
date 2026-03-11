package com.taskmanagement.dto.response;

import com.fasterxml.jackson.annotation.JsonInclude;
import com.taskmanagement.entity.Goal.GoalStatus;
import com.taskmanagement.entity.Goal.Priority;
import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.time.LocalDate;
import java.time.LocalDateTime;
import java.util.List;

@Data
@Builder
@NoArgsConstructor
@AllArgsConstructor
@JsonInclude(JsonInclude.Include.NON_NULL)
public class GoalResponse {
    private Long id;
    private String title;
    private String description;
    private GoalStatus status;
    private Priority priority;
    private LocalDate targetDate;
    private LocalDate completionDate;
    private Integer progressPercentage;
    private LocalDateTime createdAt;
    private LocalDateTime updatedAt;

    private Long userId;

    private GoalStats stats;
    private List<TaskResponse> tasks;

    @Data
    @Builder
    @NoArgsConstructor
    @AllArgsConstructor
    public static class GoalStats {
        private long totalTasks;
        private long completedTasks;
        private long pendingTasks;
        private long overdueTasks;
    }

    @Data
    @Builder
    @NoArgsConstructor
    @AllArgsConstructor
    public static class Simple {
        private Long id;
        private String title;
        private GoalStatus status;
        private Integer progressPercentage;
    }
}

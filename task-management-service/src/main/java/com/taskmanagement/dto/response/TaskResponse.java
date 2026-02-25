package com.taskmanagement.dto.response;

import com.fasterxml.jackson.annotation.JsonInclude;
import com.taskmanagement.entity.Task.Priority;
import com.taskmanagement.entity.Task.TaskStatus;
import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.time.LocalDate;
import java.time.LocalDateTime;

@Data
@Builder
@NoArgsConstructor
@AllArgsConstructor
@JsonInclude(JsonInclude.Include.NON_NULL)
public class TaskResponse {
    private Long id;
    private String title;
    private String description;
    private TaskStatus status;
    private Priority priority;
    private boolean completed;
    private LocalDate dueDate;
    private LocalDateTime completedAt;
    private Integer displayOrder;
    private Double estimatedHours;
    private Double actualHours;
    private LocalDateTime createdAt;
    private LocalDateTime updatedAt;

    private Long goalId;
    private String goalTitle;

    private boolean isOverdue;

    @Data
    @Builder
    @NoArgsConstructor
    @AllArgsConstructor
    public static class Simple {
        private Long id;
        private String title;
        private TaskStatus status;
        private boolean completed;
        private LocalDate dueDate;
    }
}

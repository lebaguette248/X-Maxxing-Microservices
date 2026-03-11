package com.taskmanagement.dto.request;

import com.taskmanagement.entity.Task.Priority;
import com.taskmanagement.entity.Task.TaskStatus;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotNull;
import jakarta.validation.constraints.Positive;
import jakarta.validation.constraints.Size;
import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.time.LocalDate;
import java.util.List;

public class TaskRequest {

    @Data
    @Builder
    @NoArgsConstructor
    @AllArgsConstructor
    public static class Create {
        @NotNull(message = "Goal ID is required")
        private Long goalId;

        @NotBlank(message = "Title is required")
        @Size(max = 255, message = "Title cannot exceed 255 characters")
        private String title;

        private String description;

        private Priority priority;

        private LocalDate dueDate;

        @Positive(message = "Estimated hours must be positive")
        private Double estimatedHours;

        private Integer displayOrder;
    }

    @Data
    @Builder
    @NoArgsConstructor
    @AllArgsConstructor
    public static class Update {
        @Size(max = 255, message = "Title cannot exceed 255 characters")
        private String title;

        private String description;

        private TaskStatus status;

        private Priority priority;

        private LocalDate dueDate;

        @Positive(message = "Estimated hours must be positive")
        private Double estimatedHours;

        @Positive(message = "Actual hours must be positive")
        private Double actualHours;

        private Integer displayOrder;
    }

    @Data
    @Builder
    @NoArgsConstructor
    @AllArgsConstructor
    public static class StatusUpdate {
        @NotNull(message = "Completed status is required")
        private Boolean completed;
    }

    @Data
    @Builder
    @NoArgsConstructor
    @AllArgsConstructor
    public static class ReorderRequest {
        @NotNull(message = "Task orders are required")
        private List<TaskOrder> taskOrders;
    }

    @Data
    @Builder
    @NoArgsConstructor
    @AllArgsConstructor
    public static class TaskOrder {
        @NotNull(message = "Task ID is required")
        private Long taskId;

        @NotNull(message = "Order is required")
        private Integer order;
    }

    @Data
    @Builder
    @NoArgsConstructor
    @AllArgsConstructor
    public static class BulkCreate {
        @NotNull(message = "Goal ID is required")
        private Long goalId;

        @NotNull(message = "Tasks are required")
        private List<TaskItem> tasks;
    }

    @Data
    @Builder
    @NoArgsConstructor
    @AllArgsConstructor
    public static class TaskItem {
        @NotBlank(message = "Title is required")
        private String title;

        private String description;

        private Priority priority;

        private LocalDate dueDate;
    }
}

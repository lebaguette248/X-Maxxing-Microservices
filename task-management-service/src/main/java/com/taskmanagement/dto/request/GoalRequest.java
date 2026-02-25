package com.taskmanagement.dto.request;

import com.taskmanagement.entity.Goal.GoalStatus;
import com.taskmanagement.entity.Goal.Priority;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotNull;
import jakarta.validation.constraints.Size;
import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.time.LocalDate;

public class GoalRequest {

    @Data
    @Builder
    @NoArgsConstructor
    @AllArgsConstructor
    public static class Create {
        @NotNull(message = "User ID is required")
        private Long userId;

        @NotBlank(message = "Title is required")
        @Size(max = 255, message = "Title cannot exceed 255 characters")
        private String title;

        private String description;

        private Priority priority;

        private LocalDate targetDate;
    }

    @Data
    @Builder
    @NoArgsConstructor
    @AllArgsConstructor
    public static class Update {
        @Size(max = 255, message = "Title cannot exceed 255 characters")
        private String title;

        private String description;

        private GoalStatus status;

        private Priority priority;

        private LocalDate targetDate;
    }

    @Data
    @Builder
    @NoArgsConstructor
    @AllArgsConstructor
    public static class StatusUpdate {
        @NotNull(message = "Status is required")
        private GoalStatus status;
    }

    @Data
    @Builder
    @NoArgsConstructor
    @AllArgsConstructor
    public static class Filter {
        private GoalStatus status;
        private Priority priority;
        private LocalDate targetDateFrom;
        private LocalDate targetDateTo;
    }
}

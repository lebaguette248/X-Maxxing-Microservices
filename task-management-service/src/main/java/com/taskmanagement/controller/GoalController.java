package com.taskmanagement.controller;

import com.taskmanagement.dto.request.GoalRequest;
import com.taskmanagement.dto.response.ApiResponse;
import com.taskmanagement.dto.response.GoalResponse;
import com.taskmanagement.entity.Goal.GoalStatus;
import com.taskmanagement.entity.Goal.Priority;
import com.taskmanagement.service.GoalService;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import lombok.RequiredArgsConstructor;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.Pageable;
import org.springframework.data.web.PageableDefault;
import org.springframework.format.annotation.DateTimeFormat;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.time.LocalDate;
import java.util.List;

@RestController
@RequestMapping("/api/v1/goals")
@RequiredArgsConstructor
@Tag(name = "Goals", description = "Goal management endpoints")
public class GoalController {

    private final GoalService goalService;

    @PostMapping
    @Operation(summary = "Create goal", description = "Create a new goal")
    public ResponseEntity<ApiResponse<GoalResponse>> createGoal(@Valid @RequestBody GoalRequest.Create request) {
        GoalResponse response = goalService.createGoal(request);
        return ResponseEntity
                .status(HttpStatus.CREATED)
                .body(ApiResponse.success("Goal created successfully", response));
    }

    @GetMapping("/{id}")
    @Operation(summary = "Get goal by ID", description = "Get goal details by ID")
    public ResponseEntity<ApiResponse<GoalResponse>> getGoalById(
            @Parameter(description = "Goal ID") @PathVariable Long id,
            @Parameter(description = "Include tasks") @RequestParam(defaultValue = "false") boolean includeTasks) {
        GoalResponse response = includeTasks 
                ? goalService.getGoalByIdWithTasks(id) 
                : goalService.getGoalById(id);
        return ResponseEntity.ok(ApiResponse.success(response));
    }

    @GetMapping("/user/{userId}")
    @Operation(summary = "Get goals by user", description = "Get all goals for a user")
    public ResponseEntity<ApiResponse<List<GoalResponse>>> getGoalsByUserId(
            @Parameter(description = "User ID") @PathVariable Long userId) {
        List<GoalResponse> goals = goalService.getGoalsByUserId(userId);
        return ResponseEntity.ok(ApiResponse.success(goals));
    }

    @GetMapping("/user/{userId}/paginated")
    @Operation(summary = "Get goals by user (paginated)", description = "Get paginated goals for a user")
    public ResponseEntity<ApiResponse<Page<GoalResponse>>> getGoalsByUserIdPaginated(
            @Parameter(description = "User ID") @PathVariable Long userId,
            @PageableDefault(size = 10) Pageable pageable) {
        Page<GoalResponse> goals = goalService.getGoalsByUserId(userId, pageable);
        ApiResponse.PageInfo pageInfo = ApiResponse.PageInfo.builder()
                .page(goals.getNumber())
                .size(goals.getSize())
                .totalElements(goals.getTotalElements())
                .totalPages(goals.getTotalPages())
                .hasNext(goals.hasNext())
                .hasPrevious(goals.hasPrevious())
                .build();
        return ResponseEntity.ok(ApiResponse.success(goals, pageInfo));
    }

    @GetMapping("/user/{userId}/filter")
    @Operation(summary = "Get filtered goals", description = "Get goals filtered by status and priority")
    public ResponseEntity<ApiResponse<Page<GoalResponse>>> getFilteredGoals(
            @Parameter(description = "User ID") @PathVariable Long userId,
            @Parameter(description = "Status filter") @RequestParam(required = false) GoalStatus status,
            @Parameter(description = "Priority filter") @RequestParam(required = false) Priority priority,
            @PageableDefault(size = 10) Pageable pageable) {
        Page<GoalResponse> goals = goalService.getGoalsByFilters(userId, status, priority, pageable);
        ApiResponse.PageInfo pageInfo = ApiResponse.PageInfo.builder()
                .page(goals.getNumber())
                .size(goals.getSize())
                .totalElements(goals.getTotalElements())
                .totalPages(goals.getTotalPages())
                .hasNext(goals.hasNext())
                .hasPrevious(goals.hasPrevious())
                .build();
        return ResponseEntity.ok(ApiResponse.success(goals, pageInfo));
    }

    @PutMapping("/{id}")
    @Operation(summary = "Update goal", description = "Update goal details")
    public ResponseEntity<ApiResponse<GoalResponse>> updateGoal(
            @Parameter(description = "Goal ID") @PathVariable Long id,
            @Valid @RequestBody GoalRequest.Update request) {
        GoalResponse response = goalService.updateGoal(id, request);
        return ResponseEntity.ok(ApiResponse.success("Goal updated successfully", response));
    }

    @PatchMapping("/{id}/status")
    @Operation(summary = "Update goal status", description = "Update goal status only")
    public ResponseEntity<ApiResponse<GoalResponse>> updateGoalStatus(
            @Parameter(description = "Goal ID") @PathVariable Long id,
            @Valid @RequestBody GoalRequest.StatusUpdate request) {
        GoalResponse response = goalService.updateGoalStatus(id, request.getStatus());
        return ResponseEntity.ok(ApiResponse.success("Goal status updated", response));
    }

    @DeleteMapping("/{id}")
    @Operation(summary = "Delete goal", description = "Delete a goal and all its tasks")
    public ResponseEntity<ApiResponse<Void>> deleteGoal(
            @Parameter(description = "Goal ID") @PathVariable Long id) {
        goalService.deleteGoal(id);
        return ResponseEntity.ok(ApiResponse.<Void>success("Goal deleted successfully", null));    }

    @GetMapping("/user/{userId}/upcoming")
    @Operation(summary = "Get upcoming deadlines", description = "Get goals with upcoming target dates")
    public ResponseEntity<ApiResponse<List<GoalResponse>>> getUpcomingDeadlines(
            @Parameter(description = "User ID") @PathVariable Long userId,
            @Parameter(description = "Until date") 
            @RequestParam(required = false) @DateTimeFormat(iso = DateTimeFormat.ISO.DATE) LocalDate untilDate) {
        LocalDate date = untilDate != null ? untilDate : LocalDate.now().plusDays(7);
        List<GoalResponse> goals = goalService.getUpcomingDeadlines(userId, date);
        return ResponseEntity.ok(ApiResponse.success(goals));
    }

    @GetMapping("/{id}/stats")
    @Operation(summary = "Get goal stats", description = "Get statistics for a goal")
    public ResponseEntity<ApiResponse<GoalResponse.GoalStats>> getGoalStats(
            @Parameter(description = "Goal ID") @PathVariable Long id) {
        GoalResponse.GoalStats stats = goalService.getGoalStats(id);
        return ResponseEntity.ok(ApiResponse.success(stats));
    }
}

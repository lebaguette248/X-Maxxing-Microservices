package com.taskmanagement.controller;

import com.taskmanagement.dto.request.TaskRequest;
import com.taskmanagement.dto.response.ApiResponse;
import com.taskmanagement.dto.response.TaskResponse;
import com.taskmanagement.service.TaskService;
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
@RequestMapping("/api/v1/tasks")
@RequiredArgsConstructor
@Tag(name = "Tasks", description = "Task management endpoints")
public class TaskController {

    private final TaskService taskService;

    @PostMapping
    @Operation(summary = "Create task", description = "Create a new task")
    public ResponseEntity<ApiResponse<TaskResponse>> createTask(@Valid @RequestBody TaskRequest.Create request) {
        TaskResponse response = taskService.createTask(request);
        return ResponseEntity
                .status(HttpStatus.CREATED)
                .body(ApiResponse.success("Task created successfully", response));
    }

    @PostMapping("/bulk")
    @Operation(summary = "Create multiple tasks", description = "Create multiple tasks at once")
    public ResponseEntity<ApiResponse<List<TaskResponse>>> createBulkTasks(
            @Valid @RequestBody TaskRequest.BulkCreate request) {
        List<TaskResponse> responses = taskService.createBulkTasks(request);
        return ResponseEntity
                .status(HttpStatus.CREATED)
                .body(ApiResponse.success("Tasks created successfully", responses));
    }

    @GetMapping("/{id}")
    @Operation(summary = "Get task by ID", description = "Get task details by ID")
    public ResponseEntity<ApiResponse<TaskResponse>> getTaskById(
            @Parameter(description = "Task ID") @PathVariable Long id) {
        TaskResponse response = taskService.getTaskById(id);
        return ResponseEntity.ok(ApiResponse.success(response));
    }

    @GetMapping("/goal/{goalId}")
    @Operation(summary = "Get tasks by goal", description = "Get all tasks for a goal")
    public ResponseEntity<ApiResponse<List<TaskResponse>>> getTasksByGoalId(
            @Parameter(description = "Goal ID") @PathVariable Long goalId) {
        List<TaskResponse> tasks = taskService.getTasksByGoalId(goalId);
        return ResponseEntity.ok(ApiResponse.success(tasks));
    }

    @GetMapping("/goal/{goalId}/paginated")
    @Operation(summary = "Get tasks by goal (paginated)", description = "Get paginated tasks for a goal")
    public ResponseEntity<ApiResponse<Page<TaskResponse>>> getTasksByGoalIdPaginated(
            @Parameter(description = "Goal ID") @PathVariable Long goalId,
            @PageableDefault(size = 20) Pageable pageable) {
        Page<TaskResponse> tasks = taskService.getTasksByGoalId(goalId, pageable);
        ApiResponse.PageInfo pageInfo = ApiResponse.PageInfo.builder()
                .page(tasks.getNumber())
                .size(tasks.getSize())
                .totalElements(tasks.getTotalElements())
                .totalPages(tasks.getTotalPages())
                .hasNext(tasks.hasNext())
                .hasPrevious(tasks.hasPrevious())
                .build();
        return ResponseEntity.ok(ApiResponse.success(tasks, pageInfo));
    }

    @GetMapping("/user/{userId}")
    @Operation(summary = "Get tasks by user", description = "Get all tasks for a user")
    public ResponseEntity<ApiResponse<List<TaskResponse>>> getTasksByUserId(
            @Parameter(description = "User ID") @PathVariable Long userId) {
        List<TaskResponse> tasks = taskService.getTasksByUserId(userId);
        return ResponseEntity.ok(ApiResponse.success(tasks));
    }

    @PutMapping("/{id}")
    @Operation(summary = "Update task", description = "Update task details")
    public ResponseEntity<ApiResponse<TaskResponse>> updateTask(
            @Parameter(description = "Task ID") @PathVariable Long id,
            @Valid @RequestBody TaskRequest.Update request) {
        TaskResponse response = taskService.updateTask(id, request);
        return ResponseEntity.ok(ApiResponse.success("Task updated successfully", response));
    }

    @PatchMapping("/{id}/complete")
    @Operation(summary = "Toggle task completion", description = "Mark task as complete or incomplete")
    public ResponseEntity<ApiResponse<TaskResponse>> toggleTaskCompletion(
            @Parameter(description = "Task ID") @PathVariable Long id,
            @Valid @RequestBody TaskRequest.StatusUpdate request) {
        TaskResponse response = taskService.toggleTaskCompletion(id, request.getCompleted());
        return ResponseEntity.ok(ApiResponse.success("Task completion updated", response));
    }

    @DeleteMapping("/{id}")
    @Operation(summary = "Delete task", description = "Delete a task")
    public ResponseEntity<ApiResponse<Void>> deleteTask(
            @Parameter(description = "Task ID") @PathVariable Long id) {
        taskService.deleteTask(id);
        return ResponseEntity.ok(ApiResponse.<Void>success("Task deleted successfully", null));    }

    @PutMapping("/reorder")
    @Operation(summary = "Reorder tasks", description = "Update the display order of tasks")
    public ResponseEntity<ApiResponse<Void>> reorderTasks(@Valid @RequestBody TaskRequest.ReorderRequest request) {
        taskService.reorderTasks(request);
        return ResponseEntity.ok(ApiResponse.<Void>success("Tasks reordered successfully", null));    }

    @GetMapping("/user/{userId}/overdue")
    @Operation(summary = "Get overdue tasks", description = "Get all overdue tasks for a user")
    public ResponseEntity<ApiResponse<List<TaskResponse>>> getOverdueTasks(
            @Parameter(description = "User ID") @PathVariable Long userId) {
        List<TaskResponse> tasks = taskService.getOverdueTasks(userId);
        return ResponseEntity.ok(ApiResponse.success(tasks));
    }

    @GetMapping("/user/{userId}/today")
    @Operation(summary = "Get today's tasks", description = "Get tasks due today for a user")
    public ResponseEntity<ApiResponse<List<TaskResponse>>> getTodaysTasks(
            @Parameter(description = "User ID") @PathVariable Long userId) {
        List<TaskResponse> tasks = taskService.getTodaysTasks(userId);
        return ResponseEntity.ok(ApiResponse.success(tasks));
    }

    @GetMapping("/user/{userId}/due-on")
    @Operation(summary = "Get tasks due on date", description = "Get tasks due on a specific date")
    public ResponseEntity<ApiResponse<List<TaskResponse>>> getTasksDueOn(
            @Parameter(description = "User ID") @PathVariable Long userId,
            @Parameter(description = "Due date") 
            @RequestParam @DateTimeFormat(iso = DateTimeFormat.ISO.DATE) LocalDate date) {
        List<TaskResponse> tasks = taskService.getTasksDueOn(userId, date);
        return ResponseEntity.ok(ApiResponse.success(tasks));
    }
}

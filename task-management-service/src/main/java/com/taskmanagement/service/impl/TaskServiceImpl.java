package com.taskmanagement.service.impl;

import com.taskmanagement.dto.request.TaskRequest;
import com.taskmanagement.dto.response.TaskResponse;
import com.taskmanagement.entity.Goal;
import com.taskmanagement.entity.Task;
import com.taskmanagement.entity.Task.Priority;
import com.taskmanagement.exception.Exceptions.*;
import com.taskmanagement.repository.GoalRepository;
import com.taskmanagement.repository.TaskRepository;
import com.taskmanagement.service.TaskService;
import lombok.RequiredArgsConstructor;
import lombok.extern.slf4j.Slf4j;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.Pageable;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.time.LocalDate;
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

@Slf4j
@Service
@RequiredArgsConstructor
@Transactional
public class TaskServiceImpl implements TaskService {

    private final TaskRepository taskRepository;
    private final GoalRepository goalRepository;

    @Override
    public TaskResponse createTask(TaskRequest.Create request) {
        log.info("Creating task '{}' for goal ID: {}", request.getTitle(), request.getGoalId());

        Goal goal = goalRepository.findById(request.getGoalId())
                .orElseThrow(() -> new ResourceNotFoundException("Goal", "id", request.getGoalId()));

        int displayOrder = request.getDisplayOrder() != null 
                ? request.getDisplayOrder() 
                : (int) taskRepository.countByGoalId(request.getGoalId());

        Task task = Task.builder()
                .title(request.getTitle())
                .description(request.getDescription())
                .priority(request.getPriority() != null ? request.getPriority() : Priority.MEDIUM)
                .dueDate(request.getDueDate())
                .estimatedHours(request.getEstimatedHours())
                .displayOrder(displayOrder)
                .goal(goal)
                .build();

        task = taskRepository.save(task);
        goal.recalculateProgress();
        goalRepository.save(goal);

        log.info("Task created successfully with ID: {}", task.getId());

        return mapToResponse(task);
    }

    @Override
    public List<TaskResponse> createBulkTasks(TaskRequest.BulkCreate request) {
        log.info("Creating {} tasks for goal ID: {}", request.getTasks().size(), request.getGoalId());

        Goal goal = goalRepository.findById(request.getGoalId())
                .orElseThrow(() -> new ResourceNotFoundException("Goal", "id", request.getGoalId()));

        int startOrder = (int) taskRepository.countByGoalId(request.getGoalId());
        List<Task> tasks = new ArrayList<>();

        for (int i = 0; i < request.getTasks().size(); i++) {
            TaskRequest.TaskItem item = request.getTasks().get(i);
            Task task = Task.builder()
                    .title(item.getTitle())
                    .description(item.getDescription())
                    .priority(item.getPriority() != null ? item.getPriority() : Priority.MEDIUM)
                    .dueDate(item.getDueDate())
                    .displayOrder(startOrder + i)
                    .goal(goal)
                    .build();
            tasks.add(task);
        }

        tasks = taskRepository.saveAll(tasks);
        goal.recalculateProgress();
        goalRepository.save(goal);

        log.info("Created {} tasks successfully", tasks.size());

        return tasks.stream()
                .map(this::mapToResponse)
                .collect(Collectors.toList());
    }

    @Override
    @Transactional(readOnly = true)
    public TaskResponse getTaskById(Long id) {
        log.debug("Fetching task with ID: {}", id);
        Task task = taskRepository.findById(id)
                .orElseThrow(() -> new ResourceNotFoundException("Task", "id", id));
        return mapToResponse(task);
    }

    @Override
    @Transactional(readOnly = true)
    public List<TaskResponse> getTasksByGoalId(Long goalId) {
        log.debug("Fetching tasks for goal ID: {}", goalId);
        return taskRepository.findByGoalIdOrderByDisplayOrderAsc(goalId).stream()
                .map(this::mapToResponse)
                .collect(Collectors.toList());
    }

    @Override
    @Transactional(readOnly = true)
    public Page<TaskResponse> getTasksByGoalId(Long goalId, Pageable pageable) {
        log.debug("Fetching tasks page for goal ID: {}, page: {}", goalId, pageable.getPageNumber());
        return taskRepository.findByGoalId(goalId, pageable).map(this::mapToResponse);
    }

    @Override
    @Transactional(readOnly = true)
    public List<TaskResponse> getTasksByUserId(Long userId) {
        log.debug("Fetching all tasks for user ID: {}", userId);
        return taskRepository.findAllByUserId(userId).stream()
                .map(this::mapToResponse)
                .collect(Collectors.toList());
    }

    @Override
    public TaskResponse updateTask(Long id, TaskRequest.Update request) {
        log.info("Updating task with ID: {}", id);

        Task task = taskRepository.findById(id)
                .orElseThrow(() -> new ResourceNotFoundException("Task", "id", id));

        if (request.getTitle() != null) {
            task.setTitle(request.getTitle());
        }
        if (request.getDescription() != null) {
            task.setDescription(request.getDescription());
        }
        if (request.getStatus() != null) {
            task.setStatus(request.getStatus());
            if (request.getStatus() == Task.TaskStatus.COMPLETED) {
                task.markAsCompleted();
            }
        }
        if (request.getPriority() != null) {
            task.setPriority(request.getPriority());
        }
        if (request.getDueDate() != null) {
            task.setDueDate(request.getDueDate());
        }
        if (request.getEstimatedHours() != null) {
            task.setEstimatedHours(request.getEstimatedHours());
        }
        if (request.getActualHours() != null) {
            task.setActualHours(request.getActualHours());
        }
        if (request.getDisplayOrder() != null) {
            task.setDisplayOrder(request.getDisplayOrder());
        }

        task = taskRepository.save(task);
        
        Goal goal = task.getGoal();
        goal.recalculateProgress();
        goalRepository.save(goal);

        log.info("Task updated successfully with ID: {}", task.getId());

        return mapToResponse(task);
    }

    @Override
    public TaskResponse toggleTaskCompletion(Long id, boolean completed) {
        log.info("Toggling completion for task ID: {} to {}", id, completed);

        Task task = taskRepository.findById(id)
                .orElseThrow(() -> new ResourceNotFoundException("Task", "id", id));

        if (completed) {
            task.markAsCompleted();
        } else {
            task.markAsIncomplete();
        }

        task = taskRepository.save(task);

        Goal goal = task.getGoal();
        goal.recalculateProgress();
        goalRepository.save(goal);

        return mapToResponse(task);
    }

    @Override
    public void deleteTask(Long id) {
        log.info("Deleting task with ID: {}", id);

        Task task = taskRepository.findById(id)
                .orElseThrow(() -> new ResourceNotFoundException("Task", "id", id));

        Goal goal = task.getGoal();
        taskRepository.deleteById(id);

        goal.recalculateProgress();
        goalRepository.save(goal);

        log.info("Task deleted successfully with ID: {}", id);
    }

    @Override
    public void reorderTasks(TaskRequest.ReorderRequest request) {
        log.info("Reordering {} tasks", request.getTaskOrders().size());

        for (TaskRequest.TaskOrder taskOrder : request.getTaskOrders()) {
            taskRepository.updateDisplayOrder(taskOrder.getTaskId(), taskOrder.getOrder());
        }

        log.info("Tasks reordered successfully");
    }

    @Override
    @Transactional(readOnly = true)
    public List<TaskResponse> getOverdueTasks(Long userId) {
        log.debug("Fetching overdue tasks for user ID: {}", userId);
        return taskRepository.findOverdueTasks(userId, LocalDate.now()).stream()
                .map(this::mapToResponse)
                .collect(Collectors.toList());
    }

    @Override
    @Transactional(readOnly = true)
    public List<TaskResponse> getTasksDueOn(Long userId, LocalDate date) {
        log.debug("Fetching tasks due on {} for user ID: {}", date, userId);
        return taskRepository.findTasksDueOn(userId, date).stream()
                .map(this::mapToResponse)
                .collect(Collectors.toList());
    }

    @Override
    @Transactional(readOnly = true)
    public List<TaskResponse> getTodaysTasks(Long userId) {
        return getTasksDueOn(userId, LocalDate.now());
    }

    private TaskResponse mapToResponse(Task task) {
        boolean isOverdue = !task.isCompleted() 
                && task.getDueDate() != null 
                && task.getDueDate().isBefore(LocalDate.now());

        return TaskResponse.builder()
                .id(task.getId())
                .title(task.getTitle())
                .description(task.getDescription())
                .status(task.getStatus())
                .priority(task.getPriority())
                .completed(task.isCompleted())
                .dueDate(task.getDueDate())
                .completedAt(task.getCompletedAt())
                .displayOrder(task.getDisplayOrder())
                .estimatedHours(task.getEstimatedHours())
                .actualHours(task.getActualHours())
                .goalId(task.getGoal().getId())
                .goalTitle(task.getGoal().getTitle())
                .isOverdue(isOverdue)
                .createdAt(task.getCreatedAt())
                .updatedAt(task.getUpdatedAt())
                .build();
    }
}

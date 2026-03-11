package com.taskmanagement.service.impl;

import com.taskmanagement.dto.request.GoalRequest;
import com.taskmanagement.dto.response.GoalResponse;
import com.taskmanagement.dto.response.TaskResponse;
import com.taskmanagement.entity.Goal;
import com.taskmanagement.entity.Goal.GoalStatus;
import com.taskmanagement.entity.Goal.Priority;
import com.taskmanagement.exception.Exceptions.*;
import com.taskmanagement.repository.GoalRepository;
import com.taskmanagement.repository.TaskRepository;
import com.taskmanagement.service.GoalService;
import lombok.RequiredArgsConstructor;
import lombok.extern.slf4j.Slf4j;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.Pageable;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.time.LocalDate;
import java.util.List;
import java.util.stream.Collectors;

@Slf4j
@Service
@RequiredArgsConstructor
@Transactional
public class GoalServiceImpl implements GoalService {

    private final GoalRepository goalRepository;
    private final TaskRepository taskRepository;

    @Override
    public GoalResponse createGoal(GoalRequest.Create request) {
        log.info("Creating goal '{}' for user ID: {}", request.getTitle(), request.getUserId());

        Goal goal = Goal.builder()
                .title(request.getTitle())
                .description(request.getDescription())
                .priority(request.getPriority() != null ? request.getPriority() : Priority.MEDIUM)
                .targetDate(request.getTargetDate())
                .userId(request.getUserId())
                .build();

        goal = goalRepository.save(goal);
        log.info("Goal created successfully with ID: {}", goal.getId());

        return mapToResponse(goal);
    }

    @Override
    @Transactional(readOnly = true)
    public GoalResponse getGoalById(Long id) {
        log.debug("Fetching goal with ID: {}", id);
        Goal goal = goalRepository.findById(id)
                .orElseThrow(() -> new ResourceNotFoundException("Goal", "id", id));
        return mapToResponse(goal);
    }

    @Override
    @Transactional(readOnly = true)
    public GoalResponse getGoalByIdWithTasks(Long id) {
        log.debug("Fetching goal with tasks for ID: {}", id);
        Goal goal = goalRepository.findByIdWithTasks(id)
                .orElseThrow(() -> new ResourceNotFoundException("Goal", "id", id));

        GoalResponse response = mapToResponse(goal);
        response.setTasks(goal.getTasks().stream()
                .map(this::mapTaskToResponse)
                .collect(Collectors.toList()));
        response.setStats(getGoalStats(id));

        return response;
    }

    @Override
    @Transactional(readOnly = true)
    public List<GoalResponse> getGoalsByUserId(Long userId) {
        log.debug("Fetching goals for user ID: {}", userId);
        return goalRepository.findByUserId(userId).stream()
                .map(this::mapToResponse)
                .collect(Collectors.toList());
    }

    @Override
    @Transactional(readOnly = true)
    public Page<GoalResponse> getGoalsByUserId(Long userId, Pageable pageable) {
        log.debug("Fetching goals page for user ID: {}, page: {}", userId, pageable.getPageNumber());
        return goalRepository.findByUserId(userId, pageable).map(this::mapToResponse);
    }

    @Override
    @Transactional(readOnly = true)
    public Page<GoalResponse> getGoalsByFilters(Long userId, GoalStatus status, Priority priority, Pageable pageable) {
        log.debug("Fetching filtered goals for user ID: {}", userId);
        return goalRepository.findByFilters(userId, status, priority, pageable).map(this::mapToResponse);
    }

    @Override
    public GoalResponse updateGoal(Long id, GoalRequest.Update request) {
        log.info("Updating goal with ID: {}", id);

        Goal goal = goalRepository.findById(id)
                .orElseThrow(() -> new ResourceNotFoundException("Goal", "id", id));

        if (request.getTitle() != null) {
            goal.setTitle(request.getTitle());
        }
        if (request.getDescription() != null) {
            goal.setDescription(request.getDescription());
        }
        if (request.getStatus() != null) {
            goal.setStatus(request.getStatus());
            if (request.getStatus() == GoalStatus.COMPLETED) {
                goal.setCompletionDate(LocalDate.now());
            }
        }
        if (request.getPriority() != null) {
            goal.setPriority(request.getPriority());
        }
        if (request.getTargetDate() != null) {
            goal.setTargetDate(request.getTargetDate());
        }

        goal = goalRepository.save(goal);
        log.info("Goal updated successfully with ID: {}", goal.getId());

        return mapToResponse(goal);
    }

    @Override
    public GoalResponse updateGoalStatus(Long id, GoalStatus status) {
        log.info("Updating status for goal ID: {} to {}", id, status);

        Goal goal = goalRepository.findById(id)
                .orElseThrow(() -> new ResourceNotFoundException("Goal", "id", id));

        goal.setStatus(status);
        if (status == GoalStatus.COMPLETED) {
            goal.setCompletionDate(LocalDate.now());
        } else {
            goal.setCompletionDate(null);
        }

        goal = goalRepository.save(goal);
        return mapToResponse(goal);
    }

    @Override
    public void deleteGoal(Long id) {
        log.info("Deleting goal with ID: {}", id);

        if (!goalRepository.existsById(id)) {
            throw new ResourceNotFoundException("Goal", "id", id);
        }

        taskRepository.deleteAllByGoalId(id);
        goalRepository.deleteById(id);
        log.info("Goal deleted successfully with ID: {}", id);
    }

    @Override
    @Transactional(readOnly = true)
    public List<GoalResponse> getUpcomingDeadlines(Long userId, LocalDate untilDate) {
        log.debug("Fetching upcoming deadlines for user ID: {} until {}", userId, untilDate);
        return goalRepository.findUpcomingDeadlines(userId, untilDate).stream()
                .map(this::mapToResponse)
                .collect(Collectors.toList());
    }

    @Override
    @Transactional(readOnly = true)
    public GoalResponse.GoalStats getGoalStats(Long goalId) {
        long totalTasks = taskRepository.countByGoalId(goalId);
        long completedTasks = taskRepository.countCompletedByGoalId(goalId);
        
        var tasks = taskRepository.findByGoalId(goalId);
        long overdueTasks = tasks.stream()
                .filter(t -> !t.isCompleted() && t.getDueDate() != null && t.getDueDate().isBefore(LocalDate.now()))
                .count();

        return GoalResponse.GoalStats.builder()
                .totalTasks(totalTasks)
                .completedTasks(completedTasks)
                .pendingTasks(totalTasks - completedTasks)
                .overdueTasks(overdueTasks)
                .build();
    }

    @Override
    @Transactional(readOnly = true)
    public long countGoalsByStatus(Long userId, GoalStatus status) {
        return goalRepository.countByUserIdAndStatus(userId, status);
    }

    private GoalResponse mapToResponse(Goal goal) {
        return GoalResponse.builder()
                .id(goal.getId())
                .title(goal.getTitle())
                .description(goal.getDescription())
                .status(goal.getStatus())
                .priority(goal.getPriority())
                .targetDate(goal.getTargetDate())
                .completionDate(goal.getCompletionDate())
                .progressPercentage(goal.getProgressPercentage())
                .userId(goal.getUserId())
                .createdAt(goal.getCreatedAt())
                .updatedAt(goal.getUpdatedAt())
                .build();
    }

    private TaskResponse mapTaskToResponse(com.taskmanagement.entity.Task task) {
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

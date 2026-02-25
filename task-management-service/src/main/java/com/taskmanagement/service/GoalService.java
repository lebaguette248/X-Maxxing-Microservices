package com.taskmanagement.service;

import com.taskmanagement.dto.request.GoalRequest;
import com.taskmanagement.dto.response.GoalResponse;
import com.taskmanagement.entity.Goal.GoalStatus;
import com.taskmanagement.entity.Goal.Priority;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.Pageable;

import java.time.LocalDate;
import java.util.List;

public interface GoalService {

    GoalResponse createGoal(GoalRequest.Create request);

    GoalResponse getGoalById(Long id);

    GoalResponse getGoalByIdWithTasks(Long id);

    List<GoalResponse> getGoalsByUserId(Long userId);

    Page<GoalResponse> getGoalsByUserId(Long userId, Pageable pageable);

    Page<GoalResponse> getGoalsByFilters(Long userId, GoalStatus status, Priority priority, Pageable pageable);

    GoalResponse updateGoal(Long id, GoalRequest.Update request);

    GoalResponse updateGoalStatus(Long id, GoalStatus status);

    void deleteGoal(Long id);

    List<GoalResponse> getUpcomingDeadlines(Long userId, LocalDate untilDate);

    GoalResponse.GoalStats getGoalStats(Long goalId);

    long countGoalsByStatus(Long userId, GoalStatus status);
}

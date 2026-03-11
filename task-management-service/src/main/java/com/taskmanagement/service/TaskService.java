package com.taskmanagement.service;

import com.taskmanagement.dto.request.TaskRequest;
import com.taskmanagement.dto.response.TaskResponse;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.Pageable;

import java.time.LocalDate;
import java.util.List;

public interface TaskService {

    TaskResponse createTask(TaskRequest.Create request);

    List<TaskResponse> createBulkTasks(TaskRequest.BulkCreate request);

    TaskResponse getTaskById(Long id);

    List<TaskResponse> getTasksByGoalId(Long goalId);

    Page<TaskResponse> getTasksByGoalId(Long goalId, Pageable pageable);

    List<TaskResponse> getTasksByUserId(Long userId);

    TaskResponse updateTask(Long id, TaskRequest.Update request);

    TaskResponse toggleTaskCompletion(Long id, boolean completed);

    void deleteTask(Long id);

    void reorderTasks(TaskRequest.ReorderRequest request);

    List<TaskResponse> getOverdueTasks(Long userId);

    List<TaskResponse> getTasksDueOn(Long userId, LocalDate date);

    List<TaskResponse> getTodaysTasks(Long userId);
}

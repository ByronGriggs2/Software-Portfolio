#pragma once
// Static class that allows saving or loading of training state. If MyExcept is fully set up when it is used, it will automatically save.
class MLP;
class ReplayBuffer;
class CurriculumManager;
class TrainingState final
{
public:
	TrainingState() = delete;
	TrainingState(const TrainingState& other) = delete;
	TrainingState(TrainingState&& other) noexcept = delete;
	TrainingState& operator=(const TrainingState& other) = delete;
	TrainingState& operator=(TrainingState&& other) noexcept = delete;

	static void loadTrainingState(MLP* DQN, MLP* targetNetwork, ReplayBuffer* replayBuffer, CurriculumManager*, float* epsilon, bool error = false);
	static void saveTrainingState(MLP* DQN, MLP* targetNetwork, ReplayBuffer* replayBuffer, const CurriculumManager*, const float* epsilon, bool error = false);
};


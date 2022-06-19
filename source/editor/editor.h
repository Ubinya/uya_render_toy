#pragma once
#include <memory>

namespace Uya {
	class EditorUI;
	class UyaEngine;

    class PilotEditor
    {
        friend class EditorUI;

    public:
        PilotEditor();
        virtual ~PilotEditor();

        void initialize(UyaEngine* engine_runtime);
        void clear();

        void run();

    protected:
        std::shared_ptr<EditorUI> m_editor_ui;
        UyaEngine* m_engine_runtime{ nullptr };
    };
}//namespace Uya
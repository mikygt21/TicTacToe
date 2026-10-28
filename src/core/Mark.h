#pragma once

namespace ttt {

    enum class Mark { 
        None = 0, 
        X, 
        O 
    };

    constexpr Mark opponent(Mark m) noexcept
    {
        return m == Mark::X ? 
            Mark::O : 
            (m == Mark::O ? Mark::X : Mark::None);
    }

}
